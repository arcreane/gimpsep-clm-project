//
// Created by charm on 19/05/2024.
//

#include "ImageApp.h"

// https://github.com/Dovyski/cvui
// https://fernandobevilacqua.com/cvui/components/image/
#include "../lib/cvui.h" // Cannot be put in the header file



/* ************************************************* */
/* Constructors */
/* ************************************************* */
ImageApp::ImageApp() {
    // Load icon Block images
    for (const auto& iconName : this->iconNameFileBlock) {
        cv::Mat icon_idle = cv::imread(this->iconFolder + iconName + "-idle.png");
        cv::Mat icon_over = cv::imread(this->iconFolder + iconName + "-over.png");
        cv::Mat icon_down = cv::imread(this->iconFolder + iconName + "-down.png");
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // faire mon traitement python d'image si l'image existe !!!!!!!!!!!!!!!!!!
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // If cannot open image then black image
        if (icon_idle.empty()) {icon_idle = cv::Mat::ones(cv::Size(40, 40), CV_8UC3); icon_idle.setTo(cv::Scalar(82, 82, 82));}
        if (icon_over.empty()) {icon_over = cv::Mat::zeros(cv::Size(40, 40), CV_8UC3);}
        if (icon_down.empty()) {icon_down = cv::Mat::ones(cv::Size(40, 40), CV_8UC3); icon_down.setTo(cv::Scalar(255, 255, 255));}
        this->iconListBlock.push_back({icon_idle, icon_over, icon_down});
    }
    // Load icon Parameters images
    for (const auto& iconName : this->iconNameFileParameters) {
        cv::Mat icon_idle = cv::imread(this->iconFolder + iconName + "-idle.png");
        cv::Mat icon_over = cv::imread(this->iconFolder + iconName + "-over.png");
        cv::Mat icon_down = cv::imread(this->iconFolder + iconName + "-down.png");
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // faire mon traitement python d'image si l'image existe !!!!!!!!!!!!!!!!!!
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // If cannot open image then black image
        if (icon_idle.empty()) {icon_idle = cv::Mat::ones(cv::Size(40, 40), CV_8UC3); icon_idle.setTo(cv::Scalar(82, 82, 82));}
        if (icon_over.empty()) {icon_over = cv::Mat::zeros(cv::Size(40, 40), CV_8UC3);}
        if (icon_down.empty()) {icon_down = cv::Mat::ones(cv::Size(40, 40), CV_8UC3); icon_down.setTo(cv::Scalar(255, 255, 255));}
        this->iconListParameters.push_back({icon_idle, icon_over, icon_down});
    }
};

bool ImageApp::openStarterImage(const std::string& imagePath) {
    this->imagePathName = imagePath;
    Image img = Image(this->imagePathName);
    this->image = new ImageHandler(img);
    /*
     * pas de controle d'image, cette partie sera modifié pour imlémentation de la class image qui gerera ce cas
     * ImageApp n'aura qu'un contructeur par défaut
     * En ouvran l'appication, il y aura un popup pour select l'image
     * On peut run l'executable avec un paramete imagePath pour ne pas avoir ce popup à l'ouverture
     * if error return false ou ouvre le popup
     */

    this->defaultValues();

    return true;
}

/* ************************************************* */
/* Deconstructors */
/* ************************************************* */

ImageApp::~ImageApp() {
    delete image;
}



/* ************************************************* */
/* Getters */
/* ************************************************* */

cv::Mat& ImageApp::getFrame() {
    return this->frame;
}






/* ************************************************* */
/* Setters */
/* ************************************************* */
void ImageApp::setOption(int newOption) {
    if (this->option != newOption) {
        //this->imageSave0 = this->imageView0.clone();
        this->option = newOption;
    }

}

void ImageApp::applyParameter(int parameter) {
    switch (parameter) {
        case SAVE: saveImage(); break;
        case NEW: newImage(); break;
        case RESET: resetImage(); break;
    }
}

void ImageApp::ControlZ() {
    this->image->ControlZ();
}

void ImageApp::ControlY() {
    this->image->ControlY();
}



/* ************************************************* */
/* Blocks */
/* ************************************************* */

void ImageApp::topLeftBlock() {
    int height = windowHeight * topBlocksHeight / 100;
    int x = 0; int y = 0;
    cvui::window(this->frame, x, y, widthBlocks, height, "Options");
    // Button creation
    for (int i = 0; i < this->messagesBlock.size(); i++) {
        int x = (i % this->buttonsPerRow) * (this->buttonWidth + 10) + 5;
        int y = (i / this->buttonsPerRow) * (this->buttonHeight + 10) + 25;
        if (cvui::button(this->frame, x, y, this->iconListBlock[i][IDLE], this->iconListBlock[i][OVER], this->iconListBlock[i][DOWN])) {
            this->setOption(i);
        }
    }
    // Info-bulles creation after cursor position checking
    for (int i = 0; i < this->messagesBlock.size(); i++) {
        int x = (i % this->buttonsPerRow) * (this->buttonWidth + 10) + 5;
        int y = (i / this->buttonsPerRow) * (this->buttonHeight + 10) + 25;
        cv::Point mouse = cvui::mouse();
        cv::Rect buttonRect(x, y, this->buttonWidth, this->buttonHeight);
        if (buttonRect.contains(mouse)) {
            cvui::printf(this->frame, mouse.x, mouse.y - 20, 0.4, 0xeeeeee, this->messagesBlock[i].c_str());
        }
    }
}

void ImageApp::bottomLeftBlock() {
    switch (this->option) {
        case BRIGHTNESS: brightnessPanel(); break;
        case ROTATE: rotatePanel(); break;
        case RESIZE: resizePanel(); break;
        case CROP: cropPanel(); break;
        case DILATATION: dilatationPanel(); break;
        case EROSION: erosionPanel(); break;
        case CANNY_EDGE: cannyEdgePanel(); break;
        case PANORAMA: panoramaPanel(); break;
    }
}

void ImageApp::centerBlock() {
    // resize image pour que sa taille soit fixe sur l'ecran si taille trop grande
    cv::Mat img = this->image->getCurrentImage().getImage();
    int x = (windowWidth / 2) - (img.cols / 2);
    int y = (windowHeight / 2) - (img.rows / 2);
    cvui::image(this->frame, x, y, img);
}

void ImageApp::topRightBlock() {
    int height = windowHeight * topBlocksHeight / 100;
    int xW = windowWidth - widthBlocks;
    int yW = 0;
    cvui::window(this->frame, xW, yW, widthBlocks, height, "Parameters"); // if image
    /*cvui::window(this->frame, 600, 0, 300, 200, "Parameters");
    cvui::window(this->frame, 600, 200, 300, 200, "Others Parameters");*/ // if vidéo

    // Button creation
    for (int i = 0; i < this->messagesParameters.size(); i++) {
        int x = (i % this->buttonsPerRow) * (this->buttonWidth + 10) + xW + 5;
        int y = (i / this->buttonsPerRow) * (this->buttonHeight + 10) + yW + 25;
        if (cvui::button(this->frame, x, y, this->iconListParameters[i][IDLE], this->iconListParameters[i][OVER], this->iconListParameters[i][DOWN])) {
            this->applyParameter(i);
        }
    }
    // Info-bulles creation after cursor position checking
    for (int i = 0; i < this->messagesParameters.size(); i++) {
        int x = (i % this->buttonsPerRow) * (this->buttonWidth + 10) + xW + 5;
        int y = (i / this->buttonsPerRow) * (this->buttonHeight + 10) + yW + 25;
        cv::Point mouse = cvui::mouse();
        cv::Rect buttonRect(x, y, this->buttonWidth, this->buttonHeight);
        if (buttonRect.contains(mouse)) {
            cvui::printf(this->frame, mouse.x, mouse.y - 20, 0.4, 0xeeeeee, this->messagesParameters[i].c_str());
        }
    }
}

void ImageApp::bottomBlock(cv::Point cursor) {
    Image img = this->image->getCurrentImage();
    int y = windowHeight-bottomBlockHeight;
    cvui::rect(frame, 0, y, windowWidth+5, bottomBlockHeight, 0x4a4a4a, 0x313131);
    cvui::beginRow(this->frame, 5, y+5, -1, -1, 50);
    cvui::printf(0.4, 0xffffff, "image size: cols = %d rows = %d", img.cols(), img.rows());
    cvui::printf(0.4, 0xffffff, "cursor: x = %d y = %d", cursor.x, cursor.y);
    cvui::endRow();
}

/* ************************************************* */
/* Panels */
/* ************************************************* */

void ImageApp::createPanelWindow(const std::string& title) {
    int x = 0;
    int y = windowHeight * topBlocksHeight / 100;
    int height = windowHeight - y;
    cvui::window(this->frame, x, y, widthBlocks, height, title);
    cvui::beginColumn(this->frame, x, y+20, widthBlocks, height, 10);
}

void ImageApp::setLastAction(int newLastAction) {
    if (this->lastAction != newLastAction) {
        this->lastAction = newLastAction;
    }
    else if (newLastAction == -1) {return;}
    else {this->image->ControlZ();}
}

void ImageApp::saveChanges() {
    if (cvui::button("Save changes")) {
        this->image->Save(this->image->getCurrentImage());
    }
}

void ImageApp::defaultValuesBtn() {
    int x = widthBlocks / 2 - 35;
    int y = windowHeight - bottomBlockHeight - 35;
    if (cvui::button(this->frame, x, y, "Default")) {
        this->defaultValues();
    }
}


void ImageApp::brightnessPanel() {
    this->createPanelWindow("Brightness Panel");
    if (cvui::trackbar(widthBlocks, &this->valuebrightness, (double)-180, (double)180)) {
        setLastAction(BRIGHTNESS);
        this->image->Brightness((int) valuebrightness);
    }
    this->saveChanges();
    this->defaultValuesBtn();
    cvui::endColumn();
}


void ImageApp::rotatePanel() {
    this->createPanelWindow("Rotate Panel");
    cv::Mat img = this->image->getCurrentImage().getImage();
    if (cvui::trackbar(this->widthBlocks, &this->valuePivot, (double)-180, (double)180)) {
        setLastAction(ROTATE);
        this->image->Rotate((int) this->valuePivot, {(int) this->centerPivotX, (int) this->centerPivotY});
    }
    cvui::trackbar(this->widthBlocks, &this->centerPivotX, (double)0, (double)img.cols);
    cvui::trackbar(this->widthBlocks, &this->centerPivotY, (double)0, (double)img.rows);
    int x = (windowWidth / 2) - (img.cols / 2);
    int y = (windowHeight / 2) - (img.rows / 2);
    cvui::rect(frame, centerPivotX+x-5, centerPivotY+y-5, 10, 10, 0xff0000);
    this->saveChanges();
    this->defaultValuesBtn();
    cvui::endColumn();
}

void ImageApp::resizePanel() {
    this->createPanelWindow("Resize Panel");
    if (cvui::trackbar(this->widthBlocks, &this->valueResize, (double)0.1, maxValue)) {
        setLastAction(RESIZE);
        this->image->Resize(this->valueResize);
    }
    this->defaultValuesBtn();
    cvui::endColumn();
}

void ImageApp::cropPanel() {
    this->createPanelWindow("Crop Panel");
    cv::Mat img = this->image->getCurrentImage().getImage();
    cvui::trackbar(this->widthBlocks, &this->startColCrop, (double)0, (double)img.cols);
    cvui::trackbar(this->widthBlocks, &this->endColCrop, (double)startColCrop+1, (double)img.cols);
    cvui::trackbar(this->widthBlocks, &this->startRowCrop, (double)0, (double)img.rows);
    cvui::trackbar(this->widthBlocks, &this->endRowCrop, (double)startRowCrop+1, (double)img.rows);
    int x = (this->windowWidth / 2) - (img.cols / 2);
    int y = (this->windowHeight / 2) - (img.rows / 2);
    cvui::rect(frame, x+startColCrop, startRowCrop+y, endColCrop-startColCrop, endRowCrop-startRowCrop, 0xff0000);
    if (cvui::button("Apply")) {
        this->image->Crop(startRowCrop, endRowCrop, startColCrop, endColCrop);
        Image imgCropped = this->image->getCurrentImage();
        startColCrop = 0;
        startRowCrop = 0;
        this->endColCrop = imgCropped.cols();
        this->endRowCrop = imgCropped.rows();
        setLastAction(-1);
    }
    this->defaultValuesBtn();
    cvui::endColumn();
}

void ImageApp::dilatationPanel() {
    this->createPanelWindow("Dilatation Panel");
    if (cvui::trackbar(widthBlocks, &this->valueDilatation, (double)1.0, (double)100.0)) {
        setLastAction(DILATATION);
        this->image->Dilatation((int) valueDilatation);
    }
    this->defaultValuesBtn();
    cvui::endColumn();
}

void ImageApp::erosionPanel() {
    this->createPanelWindow("Erosion Panel");
    if (cvui::trackbar(widthBlocks, &this->valueErosion, (double)1.0, (double)100.0)) {
        setLastAction(EROSION);
        this->image->Erosion((int) valueErosion);
    }
    this->defaultValuesBtn();
    cvui::endColumn();
}

void ImageApp::cannyEdgePanel() {
    this->createPanelWindow("Canny Edge Panel");
    cvui::trackbar(this->widthBlocks, &this->blurredValueCanny, (double)0, (double)100.0);
    cvui::trackbar(this->widthBlocks, &this->lowThresholdCanny, (double)0, (double)100.0);
    cvui::trackbar(this->widthBlocks, &this->highThresholdCanny, (double)0, (double)100.0);
    if (cvui::button("Apply")) {
        std::cout << "Not implemented" << std::endl;
        //setLastAction(-1);
        //this->image->CannyEdge((float) blurredValueCanny, (int) lowThresholdCanny, (int) highThresholdCanny);
    }
    this->defaultValuesBtn();
    cvui::endColumn();
}

void ImageApp::panoramaPanel() {
    this->createPanelWindow("Panorama Panel");
    std::cout << "Not implemented" << std::endl;
    cvui::endColumn();
}



/* ************************************************* */
/* Parameters */
/* ************************************************* */

void ImageApp::saveImage() {
    // need to read the imagePath exension to save in the right format
    imwrite (this->imageOutputPath, this->image->getCurrentImage().getImage());
}
void ImageApp::newImage() {
    //system("explorer");
    std::cout << "Not implemented" << std::endl;
}
void ImageApp::resetImage() {
    this->image->Save(this->image->getSourceImage());
    this->defaultValues();
}

void ImageApp::defaultValues() {
    cv::Mat img = this->image->getCurrentImage().getImage();
    // revoir tous les valeurs par defaut
    valuebrightness = 0;
    valuePivot = 0; centerPivotX = 0; centerPivotY = 0;
    valueResize = 0;
    startRowCrop = 0; startColCrop = 0;
    valueDilatation = 0;
    valueErosion = 0;

    // ROTATE PART
    centerPivotX = img.cols / 2;
    centerPivotY = img.rows/ 2;
    // CROP
    endColCrop = img.cols;
    endRowCrop = img.rows;
    // RESIZE
    maxValue = std::min(windowWidth / img.cols, windowHeight / img.rows);
}

