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
    this->imageSource = cv::imread(this->imagePathName);
    /*
     * pas de controle d'image, cette partie sera modifié pour imlémentation de la class image qui gerera ce cas
     * ImageApp n'aura qu'un contructeur par défaut
     * En ouvran l'appication, il y aura un popup pour select l'image
     * On peut run l'executable avec un paramete imagePath pour ne pas avoir ce popup à l'ouverture
     * if error return false ou ouvre le popup
     */
    this->imageSave = this->imageSource.clone();
    this->imageView = this->imageSource.clone();
    return true;
}


/* ************************************************* */
/* Getters */
/* ************************************************* */

cv::Mat& ImageApp::getFrame() {
    return this->frame;
}



cv::Mat ImageApp::getImageSource() {
    return this->imageSource;
}

cv::Mat ImageApp::getImageSave() {
    return this->imageSave;
}

cv::Mat ImageApp::getImageView() {
    return this->imageView;
}



/* ************************************************* */
/* Setters */
/* ************************************************* */
void ImageApp::setOption(int newOption) {
    if (this->option != newOption) {
        this->imageSave = this->imageView.clone();
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

/* ************************************************* */
/* Blocks */
/* ************************************************* */

void ImageApp::topLeftBlock() {
    cvui::window(this->frame, 0, 0, 300, 200, "Options");
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
    // centrer image
    // cvui::window(frame, 300, 0, 300, 400, "Image");
    cvui::image(this->frame, 325, 100, this->imageView);
}

void ImageApp::topRightBlock() {
    cvui::window(this->frame, 600, 0, 600, 500, "Parameters"); // if image
    /*cvui::window(this->frame, 600, 0, 300, 200, "Parameters");
    cvui::window(this->frame, 600, 200, 300, 200, "Others Parameters");*/ // if vidéo

    // Button creation
    for (int i = 0; i < this->messagesParameters.size(); i++) {
        int x = (i % this->buttonsPerRow) * (this->buttonWidth + 10) + 605;
        int y = (i / this->buttonsPerRow) * (this->buttonHeight + 10) + 25;
        if (cvui::button(this->frame, x, y, this->iconListParameters[i][IDLE], this->iconListParameters[i][OVER], this->iconListParameters[i][DOWN])) {
            this->applyParameter(i);
        }
    }
    // Info-bulles creation after cursor position checking
    for (int i = 0; i < this->messagesParameters.size(); i++) {
        int x = (i % this->buttonsPerRow) * (this->buttonWidth + 10) + 605;
        int y = (i / this->buttonsPerRow) * (this->buttonHeight + 10) + 25;
        cv::Point mouse = cvui::mouse();
        cv::Rect buttonRect(x, y, this->buttonWidth, this->buttonHeight);
        if (buttonRect.contains(mouse)) {
            cvui::printf(this->frame, mouse.x, mouse.y - 20, 0.4, 0xeeeeee, this->messagesParameters[i].c_str());
        }
    }
}

void ImageApp::bottomBlock(cv::Point cursor) {
    cvui::rect(frame, 0, 498, 902, 22, 0x4a4a4a, 0x313131);
    cvui::beginRow(this->frame, 5, 505, -1, -1, 50);
    cvui::printf(0.4, 0xffffff, "image size: cols = %d rows = %d", this->imageSave.cols, this->imageSave.rows);
    cvui::printf(0.4, 0xffffff, "cursor: x = %d y = %d", cursor.x, cursor.y);
    cvui::endRow();
}

/* ************************************************* */
/* Panels */
/* ************************************************* */

void ImageApp::brightnessPanel() {
    cvui::window(this->frame, 0, 200, 300, 300, "Brightness Panel");
    cvui::beginColumn(this->frame, 0, 220, 300, 300, 10);
    cvui::checkbox( "Checkbox label", &checked);

    cvui::endColumn();
}

void ImageApp::rotatePanel() {
    cvui::window(this->frame, 0, 200, 300, 300, "Rotate Panel");
    cvui::beginColumn(this->frame, 0, 220, 300, 300, 10);
    cvui::space(5);
    if (cvui::trackbar(300, &this->valuePivot, (double)-180, (double)180)) {
        // action sur image
        cv::Point2f center((float)this->imageSave.cols/2, (float)this->imageSave.rows/2);
        cv::Mat RotationMatrix = getRotationMatrix2D(center, this->valuePivot, 1);
        warpAffine(this->imageSave, this->imageView, RotationMatrix, this->imageSave.size());
    }
    this->defaultValueBlock(valuePivot);
    cvui::endColumn();
}

void ImageApp::resizePanel() {
    cvui::window(this->frame, 0, 200, 300, 300, "Resize Panel");
    cvui::beginColumn(this->frame, 0, 220, 300, 300, 10);

    cvui::counter(&count);
    this->defaultValueBlock(count);
    cvui::endColumn();
}

void ImageApp::cropPanel() {
    cvui::window(this->frame, 0, 200, 300, 300, "Crop Panel");
    cvui::beginColumn(this->frame, 0, 220, 300, 300, 10);
    cvui::endColumn();
}

void ImageApp::dilatationPanel() {
    cvui::window(this->frame, 0, 200, 300, 300, "Dilatation Panel");
    cvui::beginColumn(this->frame, 0, 220, 300, 300, 10);

    if (cvui::trackbar(300, &valueDilatation, (double)1.0, (double)100.0)) {
        cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size((int)valueDilatation, (int)valueDilatation), cv::Point(-1, -1));
        cv::dilate(this->imageSave, this->imageView,element, cv::Point(-1, -1), 1);
    }

    this->defaultValueBlock(valueDilatation);
    cvui::endColumn();
}

void ImageApp::erosionPanel() {
    cvui::window(this->frame, 0, 200, 300, 300, "Erosion Panel");
    cvui::beginColumn(this->frame, 0, 220, 300, 300, 10);

    if (cvui::trackbar(300, &valueErosion, (double)1.0, (double)100.0)) {
        cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size((int)valueErosion, (int)valueErosion), cv::Point(-1, -1));
        cv::erode(this->imageSave, this->imageView,element, cv::Point(-1, -1), 1);
    }
    this->defaultValueBlock(valueErosion);
    cvui::endColumn();
}

void ImageApp::cannyEdgePanel() {
    cvui::window(this->frame, 0, 200, 300, 300, "Canny Edge Panel");
    cvui::beginColumn(this->frame, 0, 220, 300, 300, 10);

    cvui::space(5);
    if (cvui::trackbar(300, &this->blurredValue, (double)0.0, (double)10.0)) {}
    cvui::trackbar(300, &this->lowThreshold, (double)0, (double)200, 1, "%.0Lf", cvui::TRACKBAR_DISCRETE, 1.0);
    cvui::trackbar(300, &this->highThreshold, (double)0, (double)200, 1, "%.0Lf", cvui::TRACKBAR_DISCRETE, 1.0);
    cvui::endColumn();
}

void ImageApp::panoramaPanel() {
    cvui::window(this->frame, 0, 200, 300, 300, "Panorama Panel");
    cvui::beginColumn(this->frame, 0, 220, 300, 300, 10);

    cvui::endColumn();
}



/* ************************************************* */
/* Parameters */
/* ************************************************* */

void ImageApp::saveImage() {
    // need to read the imagePath exension to save in the right format
    imwrite (this->imageOutputPath, this->imageView);
}
void ImageApp::newImage() {
    system("explorer");
}
void ImageApp::resetImage() {
    this->imageView = this->imageSource.clone();
    this->imageSave = this->imageSource.clone();
    // Reset Trackbar Panel Values
    this->valueDilatation =0;
    this->valueErosion = 0;
    this->valuePivot = 0;
    this->blurredValue=0; this->lowThreshold=0; this->highThreshold=0;
}

void ImageApp::defaultValueBlock(double& trackbarVariable) {
    if (cvui::button(this->frame, 112, 460, "Default")) {
        trackbarVariable = 1;
        this->imageView = this->imageSave.clone();
    }
}
