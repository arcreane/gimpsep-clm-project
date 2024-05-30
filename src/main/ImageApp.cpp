//
// Created by charm on 19/05/2024.
//

#include "ImageApp.h"



#include "../lib/cvui.h"



/* ************************************************* */
/* Constructor */
/* ************************************************* */
ImageApp::ImageApp() = default;

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
/* Getter */
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
/* Setter */
/* ************************************************* */
void ImageApp::setOption(int newOption) {
    if (this->option != newOption) {
        this->imageSave = this->imageView.clone();
        this->option = newOption;
    }

}

/* ************************************************* */
/* Block */
/* ************************************************* */

void ImageApp::topLeftBlock() {
    cvui::window(this->frame, 0, 0, 300, 200, "Options");

    // Button & grid parameters & info-bulles list
    std::vector<std::string> messages = {"Brightness", "Rotate", "Resize", "Crop", "Dilatation", "Erosion", "Canny edge", "Panorama"};
    int buttonWidth = 40;
    int buttonHeight = 40;
    int buttonsPerRow = 6;

    // Icon import
    std::vector<std::string> iconNameFile = {"brightness", "rotate", "scissors", "crop", "dilatation", "erosion", "cannyEdge", "panorama"};
    std::vector<std::vector<cv::Mat>> iconList;
    for (const auto& iconName : iconNameFile) {
        cv::Mat icon_idle = cv::imread(this->iconFolder + iconName + "-idle.png", cv::IMREAD_COLOR);
        cv::Mat icon_over = cv::imread(this->iconFolder + iconName + "-over.png", cv::IMREAD_COLOR);
        cv::Mat icon_down = cv::imread(this->iconFolder + iconName + "-down.png", cv::IMREAD_COLOR);
        iconList.push_back({icon_idle, icon_over, icon_down});
    } // no image check : the iconButton will be black

    // Button creation
    for (int i = 0; i < messages.size(); i++) {
        int x = (i % buttonsPerRow) * (buttonWidth + 10) + 5;
        int y = (i / buttonsPerRow) * (buttonHeight + 10) + 25;
        if (cvui::button(this->frame, x, y,iconList[i][0], iconList[i][1], iconList[i][2])) {
            this->setOption(i);
        }
    }
    // Info-bulles creation after cursor position checking
    for (int i = 0; i < messages.size(); i++) {
        int x = (i % buttonsPerRow) * (buttonWidth + 10) + 5;
        int y = (i / buttonsPerRow) * (buttonHeight + 10) + 25;
        cv::Point mouse = cvui::mouse();
        cv::Rect buttonRect(x, y, buttonWidth, buttonHeight);
        if (buttonRect.contains(mouse)) {
            cvui::printf(this->frame, mouse.x, mouse.y - 20, 0.4, 0xeeeeee, messages[i].c_str());
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
    cvui::window(this->frame, 600, 0, 300, 400, "Parameters");
    cvui::beginColumn(this->frame, 600, 20, 300, 400);
    cvui::space(5);
    cvui::button("save image");
    if (cvui::button("change image")) {
        system("explorer");
    }
    if (cvui::button("reset Image")) {
        this->imageView = this->imageSource.clone();
    }
    cvui::endColumn();
}

void ImageApp::bottomBlock(cv::Point cursor) {
    cvui::beginRow(this->frame, 0, 500);
    cvui::printf(0.4, 0x00ff00, "cursor: x = %d y = %d", cursor.x, cursor.y);
    cvui::endRow();
}

/* ************************************************* */
/* Panel */
/* ************************************************* */

void ImageApp::brightnessPanel() {
    cvui::window(this->frame, 0, 200, 300, 200, "brightnessPanel");
    cvui::beginColumn(this->frame, 0, 220, 300, 200, 10);
    cvui::endColumn();
}

void ImageApp::rotatePanel() {
    cvui::window(this->frame, 0, 200, 300, 200, "rotatePanel");
    cvui::beginColumn(this->frame, 0, 220, 300, 200, 10);
    cvui::space(5);
    if (cvui::trackbar(300, &this->valuePivot, (double)-180, (double)180)) {
        // action sur image
        cv::Point2f center((float)this->imageSave.cols/2, (float)this->imageSave.rows/2);
        cv::Mat RotationMatrix = getRotationMatrix2D(center, this->valuePivot, 1);
        warpAffine(this->imageSave, this->imageView, RotationMatrix, this->imageSave.size());
    }
    cvui::endColumn();
}

void ImageApp::resizePanel() {
    cvui::window(this->frame, 0, 200, 300, 200, "Actions");
    cvui::beginColumn(this->frame, 0, 220, 300, 200, 10);
    cvui::text("resizePanel");
    cvui::endColumn();
}

void ImageApp::cropPanel() {
    cvui::window(this->frame, 0, 200, 300, 200, "Actions");
    cvui::beginColumn(this->frame, 0, 220, 300, 200, 10);
    cvui::text("cropPanel");
    cvui::endColumn();
}

void ImageApp::dilatationPanel() {
    cvui::window(this->frame, 0, 200, 300, 200, "Actions");
    cvui::beginColumn(this->frame, 0, 220, 300, 200, 10);
    cvui::text("dilatationPanel");
    cvui::space(5);
    if (cvui::trackbar(300, &valueDilatation, (double)1.0, (double)100.0)) {
        cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size((int)valueDilatation, (int)valueDilatation), cv::Point(-1, -1));
        cv::dilate(this->imageSave, this->imageView,element, cv::Point(-1, -1), 1);
    }
    if (cvui::button("Default")) {
        valueDilatation = 1;
        valueErosion = 1;
        this->imageView = this->imageSave.clone();
    }

    cvui::endColumn();
}

void ImageApp::erosionPanel() {
    cvui::window(this->frame, 0, 200, 300, 200, "Actions");
    cvui::beginColumn(this->frame, 0, 220, 300, 200, 10);
    cvui::text("erosionPanel");
    cvui::space(5);
    if (cvui::trackbar(300, &valueErosion, (double)1.0, (double)100.0)) {
        cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size((int)valueErosion, (int)valueErosion), cv::Point(-1, -1));
        cv::erode(this->imageSave, this->imageView,element, cv::Point(-1, -1), 1);
    }
    if (cvui::button("Default")) {
        valueDilatation = 1;
        valueErosion = 1;
        this->imageView = this->imageSave.clone();
    }
    cvui::endColumn();
}

void ImageApp::cannyEdgePanel() {
    cvui::window(this->frame, 0, 200, 300, 300, "Actions");
    cvui::beginColumn(this->frame, 0, 220, 300, 300, 10);
    cvui::text("cannyEdgePanel");
    cvui::space(5);
    if (cvui::trackbar(300, &this->blurredValue, (double)0.0, (double)10.0)) {}
    cvui::trackbar(300, &this->lowThreshold, (double)0, (double)200, 1, "%.0Lf", cvui::TRACKBAR_DISCRETE, 1.0);
    cvui::trackbar(300, &this->highThreshold, (double)0, (double)200, 1, "%.0Lf", cvui::TRACKBAR_DISCRETE, 1.0);
    cvui::endColumn();
}

void ImageApp::panoramaPanel() {
    cvui::window(this->frame, 0, 200, 300, 200, "Actions");
    cvui::beginColumn(this->frame, 0, 220, 300, 200, 10);
    cvui::text("panoramaPanel");
    cvui::endColumn();
}





