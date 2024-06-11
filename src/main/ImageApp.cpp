//
// Created by charm on 19/05/2024.
//

// One (and only one) of your C++ files must define CVUI_IMPLEMENTATION
// before the inclusion of cvui.h to ensure its implementaiton is compiled.


#include "ImageApp.h"

// https://github.com/Dovyski/cvui
// https://fernandobevilacqua.com/cvui/components/image/
#define CVUI_IMPLEMENTATION
#include "../lib/cvui.h" // Cannot be put in the header file
//#include "../lib/EnhancedWindow.h" // exemple : https://github.com/Dovyski/cvui/blob/master/example/src/ui-enhanced-window-component/main.cpp

#include "../lib/tinyfiledialogs.h" // Include file dialog library
#include <locale>




/* ************************************************* */
/* Constructors */
/* ************************************************* */
ImageApp::ImageApp() {
    // Load icon Block images
    for (const auto& iconName : this->iconNameFileBlock) {
        cv::Mat icon_idle = cv::imread(this->iconFolder + iconName + "-idle.png");
        cv::Mat icon_over = cv::imread(this->iconFolder + iconName + "-over.png");
        cv::Mat icon_down = cv::imread(this->iconFolder + iconName + "-down.png");
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
    if(img.getImage().empty()){return false;}
    // resize image if too big
    maxValue = std::min((10 * windowWidth / img.cols()) / 10.0, (10 * windowHeight / img.rows()) / 10.0);
    if (img.rows() >= windowHeight || img.cols() >= windowWidth) {
        img = img.Resize(maxValue);
    }
    if (image == nullptr) {this->image = new ImageHandler(img);}
    else {image->Save(img);}
    fileExtension = std::filesystem::path(imagePath).extension().string();
    isVideo = false;
    isVideoRunning = false;

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
    positionVideo = 0;

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


void ImageApp::startDefaultImage(std::vector<std::string> text) {
    this->imagePathName = "error.png";
    // Créer une image noire
    cv::Mat m = cv::Mat::ones(cv::Size(windowWidth/2, windowHeight/2), CV_8UC3);
    // Définir le texte sur plusieurs lignes
    if (text.empty()) {text.emplace_back("Please open a media");}
    // Définir la police et la taille
    int police = cv::FONT_HERSHEY_SIMPLEX;
    double taille = 1.0;
    // Définir la couleur (BGR) et l'épaisseur
    cv::Scalar couleur = cv::Scalar(255, 255, 255);
    int epaisseur = 2;
    // Calculer la hauteur de ligne
    int baseline = 0;
    cv::Size tailleTexte = cv::getTextSize("Test", police, taille, epaisseur, &baseline);
    int hauteurLigne = tailleTexte.height + baseline;
    // Dessiner chaque ligne
    for (int i = 0; i < text.size(); ++i) {
        // Calculer la position centrée pour cette ligne
        tailleTexte = cv::getTextSize(text[i], police, taille, epaisseur, &baseline);
        cv::Point position;
        position.x = (m.cols - tailleTexte.width) / 2;
        position.y = (int) (m.rows + hauteurLigne * (i*2 - (int) text.size() / 2.0)) / 2;
        // Ajouter le texte à l'image
        cv::putText(m, text[i], position, police, taille, couleur, epaisseur);
    }

    Image img = Image(m);
    if (image == nullptr) {this->image = new ImageHandler(img);}
    else {image->Save(img);}
    fileExtension = std::filesystem::path(imagePathName).extension().string();
    isVideo = false;
    isVideoRunning = false;
    this->defaultValues();
}


int ImageApp::run_application() {
    std::string imagePath;
    return ImageApp::run_application(imagePath);
}


int ImageApp::run_application(const std::string& imagePath) {
    std::cout << "Run application" << std::endl;
    // Create the GUI application
    ImageApp myApp = ImageApp();

    // if parameter
    bool isFileOpen = false;
    if (!imagePath.empty()) {
        isFileOpen = true;
        // try loading the image
        if (!myApp.isVideoFile(imagePath) && !myApp.openStarterImage(imagePath)){
            myApp.startDefaultImage({"Error : can't open your image.","Please select a new media"});
        }
            // try loading the video
        else if (myApp.isVideoFile(imagePath) && !myApp.openVideo(imagePath)){
            myApp.startDefaultImage({"Error : can't open your video.","Please select a new media"});
        }
    }
    // if no file is openning then default image
    if (!isFileOpen) {myApp.startDefaultImage({});}


    cvui::init(WINDOW_NAME);
    while (true) {
        myApp.getFrame() = cv::Scalar(49, 52, 49);
        cv::Point cursor = cvui::mouse();
        //---------------------------------------------------//
        if (myApp.getIsVideo() && myApp.getIsVideoRunning()) {
            myApp.showVideo();
        }
        myApp.centerBlock();
        if (!myApp.getIsVideoRunning()) {
            myApp.topLeftBlock();
            myApp.bottomLeftBlock();
        }
        myApp.topRightBlock();
        myApp.bottomBlock(cursor);

        //---------------------------------------------------//
        cvui::update();
        cvui::imshow(WINDOW_NAME, myApp.getFrame());
        char key = (char)cv::waitKey(20);
        if (key  == 27 || cv::getWindowProperty(WINDOW_NAME, cv::WND_PROP_VISIBLE) < 1) {break;}
        if (key  == 26) {myApp.ControlZ();}
        if (key  == 25) {myApp.ControlY();}
    }
    return 0;
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
        case NEW:
            newMedia(); break;
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
    cvui::text(this->fileExtension, 0.4, 0xffffff);
    // afficher la position de la sourie dans l'image
    int x1 = (windowWidth / 2) - (img.cols() / 2);
    int y1 = (windowHeight / 2) - (img.rows() / 2);
    int x2 = x1 + img.cols();
    int y2 = y1 + img.rows();
    if (x1 <= cursor.x && cursor.x <= x2 && y1 <= cursor.y && cursor.y <= y2) {
        cvui::printf(0.4, 0xffffff, "cursor: x = %d y = %d", cursor.x-x1, cursor.y-y1);
    }
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

    char const * lFilterPatterns[5] = {"*.png", "*.jpg", "*.jpeg", "*.jpe", "*.webp"};
    char const * inputPathCStr = tinyfd_openFileDialog(
            "Choose images for the panorama",
            "Downloads", //"C:\\",
            5, lFilterPatterns, nullptr, true
    );
    // Nothing si l'user ferme la fenetre
    if (inputPathCStr == nullptr) {return;}
    // Recuperation des paths divised  par '|'
    std::vector<std::string> listImages = {};
    std::stringstream ss(inputPathCStr);
    std::string token;
    while (std::getline(ss, token, '|')) {
        listImages.push_back(token);
    }
    // Apply the stitchning
    Image panorama = PanoramaCreator::CreatePanorama(listImages);

    /* // with a folder selection
    std::filesystem::path currentPath = std::filesystem::current_path();
    char const * inputPathCStr = tinyfd_selectFolderDialog(
            "Choose a folder for the panorama","Downloads");
    if (inputPathCStr == nullptr) {return;}
    std::string folderPath(inputPathCStr);
    this->imagePathName = folderPath + "/";
    PanoramaCreator myPano(this->imagePathName);
    Image panorama = PanoramaCreator::CreatePanorama(myPano.getListImages());*/

    if (panorama.getImage().empty()) {
        this->startDefaultImage({"Error : can't stitch images.","Please select a new media"});
        return;
    }
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
    char const * lFilterPatterns[5] = {"*.png", "*.jpg", "*.jpeg", "*.jpe", "*.webp"};
    char const * inputPathCStr = tinyfd_saveFileDialog(
            "Save file | Please do not select file with accent",
           "Downloads", //"C:\\",
            5, lFilterPatterns, nullptr
    );
    // Nothing si l'user ferme la fenetre
    if (inputPathCStr == nullptr) {return;}

    // Recuperation du path du file
    std::string outputPath(inputPathCStr);
    std::string fileExtension = outputPath.substr(outputPath.find_last_of('.'));

    if (videoExtensions.find(fileExtension) != videoExtensions.end()) { // video
        std::cout << "Video save not implemented. The screen hase beeen saved" << std::endl;
        imwrite (outputPath  + ".png", this->image->getCurrentImage().getImage());

    }
    else {
        imwrite (outputPath , this->image->getCurrentImage().getImage());
    }
}

void ImageApp::newMedia() {
    // boite de dialogue pour choisir un fichier
    char const * lFilterPatterns[7] = {"*.jpg", "*.png", "*.jpeg", "*.jpe", "*.webp" , "*.mp4", "*.avi" };
    char const * inputPathCStr = tinyfd_openFileDialog(
            "Select file | Please do not select file with accent",
            "Downloads", //"C:\\",
            7, lFilterPatterns, nullptr, 0
    );
    // Nothing si l'user ferme la fenetre
    if (inputPathCStr == nullptr) {return;}

    // Recuperation du path du file
    std::string inputPath(inputPathCStr);
    std::cout << inputPath << std::endl;

    // new Image handler
    delete image;
    image = nullptr;

    if (has_accents(inputPath)) {
        //std::cout << "Please do not use file with accent" << std::endl;
        //newImage(); // Call newMedia() recursively to select a new file
        this->startDefaultImage({"Error : do not use file with accent.","Please select a new media"});
        return;
    }
    if (isVideoFile(inputPath)) {
        // Call video opening method
        if (!openVideo(inputPath)) {
            //std::cout << "Error during video opening, please select new video" << std::endl;
            //newImage(); // Call newMedia() recursively to select a new file
            this->startDefaultImage({"Error : can't open your video.","Please select a new media"});
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

bool ImageApp::isVideoFile(const std::string& filename) {
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
        // resize image if too big
        maxValue = std::min((10 * windowWidth / img.cols()) / 10.0, (10 * windowHeight / img.rows()) / 10.0);
        if (img.rows() >= windowHeight || img.cols() >= windowWidth) {
            img = img.Resize(maxValue);
        }
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

bool ImageApp::getIsVideo() const {
    return isVideo;
}
bool ImageApp::getIsVideoRunning() const {
    return isVideoRunning;
}

void ImageApp::captureVideoPanel() {
    // new Image handler
    delete image;
    image = nullptr;
    this->openVideo();
}



/* ************************************************* */
/* Utils */
/* ************************************************* */

bool ImageApp::has_accents(std::string str) {
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