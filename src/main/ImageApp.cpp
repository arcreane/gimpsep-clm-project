//
// Created by charm on 19/05/2024.
//

#include "ImageApp.h"

// https://github.com/Dovyski/cvui
// https://fernandobevilacqua.com/cvui/components/image/
#include "../lib/cvui.h" // Cannot be put in the header file
#include "../lib/tinyfiledialogs.h" // Include file dialog library
#include <locale>


/* ************************************************* */
/* Utils */
/* ************************************************* */

bool has_accents(std::string str) {
    std::locale loc("en_US.UTF-8");
    for (char c : str) {
        if (std::use_facet<std::ctype<char>>(loc).is(std::ctype_base::space, c) ||
            std::use_facet<std::ctype<char>>(loc).is(std::ctype_base::print, c) ||
            std::use_facet<std::ctype<char>>(loc).is(std::ctype_base::punct, c))
        {
            // Le caractère est un espace, un caractère imprimable ou un caractère de ponctuation
            continue;
        }
        // Le caractère n'est pas un espace, un caractère imprimable ou un caractère de ponctuation, donc c'est un caractère accentué
        return true;
    }
    // Aucun caractère accentué n'a été trouvé
    return false;
}

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
    if (image == nullptr) {this->image = new ImageHandler(img);}
    else {image->Save(img);}
    fileExtension = std::filesystem::path(imagePath).extension().string();
    isVideo = false;
    isVideoRunning = false;
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

bool ImageApp::openVideo(const std::string& imagePath) {
    // close the last video
    if (cap.isOpened()) {cap.release();}
    // open the new video
    this->imagePathName = imagePath;
    fileExtension = std::filesystem::path(imagePath).extension().string();
    isVideo = true;
    isVideoRunning = true;
    cap.open(imagePath); // LINUX: cap.open(imagePath, cv::CAP_V4L2);
    if(!cap.isOpened()){return false;}

    frameCount = cap.get(cv::CAP_PROP_FRAME_COUNT);
    fps = cap.get(cv::CAP_PROP_FPS);
    videoLength = frameCount / fps;

    cv::Mat frameVideo;
    cap >> frameVideo;
    Image img = Image(frameVideo);
    if (image == nullptr) {this->image = new ImageHandler(img);}
    else {image->Save(img);}
    isVideoCapture = true;

    this->defaultValues();
    return true;
}

bool ImageApp::openVideo() {
    // close the last video
    if (cap.isOpened()) {cap.release();}
    // open the new video
    this->imagePathName = imageOutputPath +"_camera.avi";
    fileExtension = std::filesystem::path(imagePathName).extension().string();
    isVideo = true;
    isVideoRunning = true;
    cap.open(0);
    if(!cap.isOpened()){return false;}
    isVideoCapture = false;


    cv::Mat frameVideo;
    cap >> frameVideo;
    Image img = Image(frameVideo);
    if (image == nullptr) {this->image = new ImageHandler(img);}
    else {image->Save(img);}

    this->defaultValues();
    return true;
}

/* ************************************************* */
/* Deconstructors */
/* ************************************************* */

ImageApp::~ImageApp() {
    delete image;

    if (isVideo) {
        cap.release();
        cv::destroyAllWindows();
    }
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
        case PANORAMA: panoramaPanel(); break;
        case CAPTUREVIDEO: captureVideoPanel(); break;
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
    if (isVideo) {
        cvui::window(this->frame, xW, yW, widthBlocks, height, "Parameters");
        cvui::window(this->frame, xW, height, widthBlocks, windowHeight-height, "Video Panel");
        cvui::beginColumn(this->frame, xW, height+20, widthBlocks, windowHeight-height);
        this->videoPanel();
    }
    else{
        cvui::window(this->frame, xW, yW, widthBlocks, windowHeight, "Parameters");
    }
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
            cvui::printf(this->frame, mouse.x - 60, mouse.y - 20, 0.4, 0xeeeeee, this->messagesParameters[i].c_str());
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
    cvui::text(this->fileExtension, 0.4, 0xffffff);
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
    cvui::beginColumn(this->frame, x, y+25, widthBlocks, height, 10);
}

void ImageApp::setLastAction(int newLastAction) {
    if (this->lastAction != newLastAction) {
        this->lastAction = newLastAction;
        this->defaultValues();
    }
    else if (newLastAction == -1 || newLastAction == CROP) {return;}
    else {this->image->ControlZ();}
}

void ImageApp::saveChanges() {
    if (cvui::button("Save changes")) {
        this->image->Save(this->image->getCurrentImage());
        this->defaultValues();
    }
}

void ImageApp::defaultValuesBtn(bool applyCtrlZ, double value, double valueInit) {
    int x = widthBlocks / 2 - 35;
    int y = windowHeight - bottomBlockHeight - 35;
    if (cvui::button(this->frame, x, y, "Default")) {
        // apply ctrl+Z
        if (applyCtrlZ && value != valueInit) {this->image->ControlZ();}
        this->defaultValues();
    }
}


void ImageApp::brightnessPanel() {
    this->createPanelWindow("Brightness Panel");
    cvui::text("Brightness value:");
    if (cvui::trackbar(widthBlocks, &this->valuebrightness, -250, 250, 1,"%.0Lf")) {
        setLastAction(BRIGHTNESS);
        this->image->Brightness((int) valuebrightness);
    }
    this->saveChanges();
    this->defaultValuesBtn(true, this->valuebrightness, 0);
    cvui::endColumn();
}


void ImageApp::rotatePanel() {
    this->createPanelWindow("Pivot Panel");
    cv::Mat img = this->image->getCurrentImage().getImage();
    cvui::text("Pivot value:");
    if (cvui::trackbar(this->widthBlocks, &this->valuePivot, 0, 360, 1,"%.0Lf")) {
        setLastAction(ROTATE);
        this->image->Rotate((int) this->valuePivot, {(int) this->centerPivotX, (int) this->centerPivotY});
    }
    cvui::text("Pivot center X:");
    cvui::trackbar(this->widthBlocks, &this->centerPivotX, 0, img.cols, 1,"%.0Lf");
    cvui::text("Pivot center Y:");
    cvui::trackbar(this->widthBlocks, &this->centerPivotY, 0, img.rows, 1,"%.0Lf");
    int x = (windowWidth / 2) - (img.cols / 2);
    int y = (windowHeight / 2) - (img.rows / 2);
    cvui::rect(frame, centerPivotX+x-5, centerPivotY+y-5, 10, 10, 0xff0000);
    this->saveChanges();
    this->defaultValuesBtn(true, this->valuePivot, 0);
    cvui::endColumn();
}

void ImageApp::resizePanel() {
    this->createPanelWindow("Resize Panel");
    cvui::text("Resize value:");
    if (cvui::trackbar(this->widthBlocks, &this->valueResize, (double)0.1, maxValue)) {
        setLastAction(RESIZE);
        this->image->Resize(this->valueResize);
    }
    this->defaultValuesBtn(true, this->valueResize, 1);
    cvui::endColumn();
}

void ImageApp::cropPanel() {
    this->createPanelWindow("Crop Panel");
    setLastAction(CROP);
    cv::Mat img = this->image->getCurrentImage().getImage();
    int step = 2;
    if (img.cols >step) {
        cvui::text("column min value:");
        if (cvui::trackbar(this->widthBlocks, &this->startColCrop, 0, img.cols-step, 1,"%.0Lf")) {
            if (startColCrop > endColCrop) {endColCrop = startColCrop+step;}
        }
        cvui::text("column max value:");
        if (cvui::trackbar(this->widthBlocks, &this->endColCrop, 0+step, img.cols, 1,"%.0Lf")) {
            if (endColCrop < startColCrop) {startColCrop = endColCrop-step;}
        }
    }
    else {cvui::text("column value cannot be crop !");}
    if (img.rows >step) {
        cvui::text("row min value:");
        if (cvui::trackbar(this->widthBlocks, &this->startRowCrop, 0, img.rows - step, 1, "%.0Lf")) {
            if (startRowCrop > endRowCrop) { endRowCrop = startRowCrop + step; }
        }
        cvui::text("row max value:");
        if (cvui::trackbar(this->widthBlocks, &this->endRowCrop, 0 + step, img.rows, 1, "%.0Lf")) {
            if (endRowCrop < startRowCrop) { startRowCrop = endRowCrop - step; }
        }
    }
    else {cvui::text("row value cannot be crop !");}
    int x = (this->windowWidth / 2) - (img.cols / 2);
    int y = (this->windowHeight / 2) - (img.rows / 2);
    cvui::rect(frame, x+startColCrop, startRowCrop+y, endColCrop-startColCrop, endRowCrop-startRowCrop, 0xff0000);
    if (img.cols >step || img.rows >step) {
        if (cvui::button("Apply")) {
            this->image->Crop(startRowCrop, endRowCrop, startColCrop, endColCrop);
            Image imgCropped = this->image->getCurrentImage();
            this->defaultValues();
        }
        this->defaultValuesBtn(false, 0, 0);
    }
    cvui::endColumn();
}

void ImageApp::dilatationPanel() {
    this->createPanelWindow("Dilatation Panel");
    cvui::text("Dilatation value:");
    if (cvui::trackbar(widthBlocks, &this->valueDilatation, 1, 99, 1, "%.0Lf")) {
        setLastAction(DILATATION);
        this->image->Dilatation((int) valueDilatation);
    }
    this->defaultValuesBtn(true, this->valueDilatation, 1);
    cvui::endColumn();
}

void ImageApp::erosionPanel() {
    this->createPanelWindow("Erosion Panel");
    cvui::text("Erosion value:");
    if (cvui::trackbar(widthBlocks, &this->valueErosion, 1, 99, 1, "%.0Lf")) {
        setLastAction(EROSION);
        this->image->Erosion((int) valueErosion);
    }
    this->defaultValuesBtn(true, this->valueErosion, 1);
    cvui::endColumn();
}

void ImageApp::cannyEdgePanel() {
    this->createPanelWindow("Canny Edge Panel");
    cvui::text("Blurred value:");
    cvui::trackbar(this->widthBlocks, &this->blurredValueCanny, (double)0, (double)1.0);
    cvui::text("Low threshold value:");
    if (cvui::trackbar(this->widthBlocks, &this->lowThresholdCanny, 0, 255, 1, "%.0Lf")) {
        if (lowThresholdCanny > highThresholdCanny) {highThresholdCanny = lowThresholdCanny;}
    }
    cvui::text("High threshold value:");
    if (cvui::trackbar(this->widthBlocks, &this->highThresholdCanny, 0, 255, 1, "%.0Lf")) {
        if (highThresholdCanny < lowThresholdCanny) {lowThresholdCanny = highThresholdCanny;}
    }
    if (cvui::button("Apply")) {
        this->image->CannyEdge((float) blurredValueCanny, (int) lowThresholdCanny, (int) highThresholdCanny);
    }
    this->defaultValuesBtn(false, 0, 0);
    cvui::endColumn();
}

void ImageApp::panoramaPanel() {
    std::string folderPath = tinyfd_selectFolderDialog("Choose a folder for the panorama", "C:\\");

    if (folderPath.empty()) {
        std::cout << "User closed the dialog without selecting a folder." << std::endl;
        return;
    }

    std::cout << folderPath << std::endl;

    this->imagePathName = folderPath + "/";
    PanoramaCreator myPano(this->imagePathName);
    Image panorama = myPano.CreatePanorama(myPano.getListImages());

    // new Image handler
    delete image;
    image = nullptr;
    this->image = new ImageHandler(panorama);
    this->defaultValues();
    // change this->image->imageSource
    isVideo = false;
    isVideoRunning = false;
    fileExtension = ".png";
}




/* ************************************************* */
/* Parameters */
/* ************************************************* */

void ImageApp::saveImage() {
    // need to read the imagePath extension to save in the right format
    char const * lFilterPatterns[6] = {  "*.jpg", "*.png", "*.jpeg", "*.jpe" , "*.mp4", "*.avi" };
    std::string outputPath = tinyfd_saveFileDialog(
            "Save file | Please do not select file with accent",
            "C:\\",
            6,
            lFilterPatterns,
            NULL
    );

    if (outputPath.empty()) {
        std::cout << "User closed the dialog without selecting a file." << std::endl;
        return;
    }

    std::string fileExtension = outputPath.substr(outputPath.find_last_of('.'));

    if (videoExtensions.find(fileExtension) != videoExtensions.end()) { // video
        std::cout << "Video save not implemented. The screen hase beeen saved" << std::endl;
        imwrite (outputPath  + ".png", this->image->getCurrentImage().getImage());

    }
    else { // imaage
        imwrite (outputPath , this->image->getCurrentImage().getImage());

    }
}

void ImageApp::newImage() {
    char const * lFilterPatterns[6] = {  "*.jpg", "*.png", "*.jpeg", "*.jpe" , "*.mp4", "*.avi" };
    std::string inputPath = tinyfd_openFileDialog(
            "Select file | Please do not select file with accent",
            "C:\\",
            6,
            lFilterPatterns,
            NULL,
            0
    );

    std::cout << inputPath << std::endl;

    if (has_accents(inputPath)) {
        std::cout << "Please do not use file with accent" << std::endl;
        newImage(); // Call newImage() recursively to select a new file
        return;
    }

    // new Image handler
    delete image;
    image = nullptr;

    if (isVideoFile(inputPath)) {
        // Call video opening method
        if (!openVideo(inputPath)) {
            std::cout << "Error during video opening, please select new video" << std::endl;
            newImage(); // Call newImage() recursively to select a new file
        }
    } else {
        // Call img opening method
        openStarterImage(inputPath);
    }

}


void ImageApp::resetImage() {
    this->image->Save(this->image->getSourceImage());
    lastAction = -1;
    this->defaultValues();
}

void ImageApp::defaultValues() {
    cv::Mat img = this->image->getCurrentImage().getImage();
    // revoir tous les valeurs par defaut
    valuebrightness = 0;
    valuePivot = 0;  centerPivotX = img.cols / 2; centerPivotY = img.rows/ 2;
    valueResize = 1;
    maxValue = std::min((10 * windowWidth / img.cols) / 10.0, (10 * windowHeight / img.rows) / 10.0);
    startRowCrop = 0; startColCrop = 0; endColCrop = img.cols; endRowCrop = img.rows;
    valueDilatation = 1;
    valueErosion = 1;
    blurredValueCanny = 0; lowThresholdCanny = 0; highThresholdCanny = 0;
}



/* ************************************************* */
/* Video */
/* ************************************************* */

bool ImageApp::isVideoFile(std::string filename) {
    std::string extension = std::filesystem::path(filename).extension().string();
    if (videoExtensions.find(extension) != videoExtensions.end()) {
        return true;}
    return false;

}

void ImageApp::showVideo() {
    cv::Mat frameVideo;
    // Capture frame-by-frame
    positionVideo++;
    cap >> frameVideo;
    // If the frame is empty, reset video to beginning
    if (frameVideo.empty()) {
        //isVideoRunning = false;
        cap.set(cv::CAP_PROP_POS_FRAMES, 0);
        cap >> frameVideo;
        positionVideo = 0;
    }
    else {
        Image img = Image(frameVideo);
        image->Save(img);
    }
}


void ImageApp::videoPanel() {
    cvui::checkbox("run", &isVideoRunning);
    if (isVideoCapture) {
        if (cvui::trackbar(widthBlocks, &positionVideo, 0, frameCount)) {
            cap.set(cv::CAP_PROP_POS_FRAMES, positionVideo);
            this->showVideo();
        }
    }


    cvui::endColumn();
}

bool ImageApp::getIsVideo() {
    return isVideo;
}
bool ImageApp::getIsVideoRunning() {
    return isVideoRunning;
}

void ImageApp::captureVideoPanel() {
    // new Image handler
    delete image;
    image = nullptr;
    this->openVideo();
}




