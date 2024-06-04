#include "Image.h"
#include "PanoramaCreator.h"
#include "ImageHandler.h"
#include <string>

int main()
{
	std::string imagePath = "../src/ressources/HappyFish.jpg";
	std::string folderPath = "../src/ressources/stitching/";

	Image img(imagePath);	

    ImageHandler imgHandler(img);
    imgHandler.Display();

    // Apply brightness adjustment
    imgHandler.Brightness(50);
    imgHandler.Display();

    // Rotate the image by 45 degrees around the center
    std::vector<int> centerPoints = { img.cols() / 2, img.rows() / 2 };
    imgHandler.Rotate(45, centerPoints);
    imgHandler.Display();

    // Apply Canny edge 
    float blurredValue = 0.5;
    int lowThreshold = 100;
    int highThreshold = 200;  
    imgHandler.CannyEdge(blurredValue, lowThreshold, highThreshold); 
    imgHandler.Display();


    // Resize the image to half its size
    imgHandler.Resize(2.5);
    imgHandler.Display();

    // Crop the image to a central region
    int startRow = 600;
    int endRow = 200;
    int startCol = 0;
    int endCol = 3 * img.cols();
    imgHandler.Crop(startRow, endRow, startCol, endCol);
    imgHandler.Display();


    // Apply dilation
    imgHandler.Dilatation(13);
    imgHandler.Display();

    // Apply erosion
    imgHandler.Erosion(13);
    imgHandler.Display();

    // Undo the last operation (erosion)
    imgHandler.ControlZ();
    imgHandler.Display();

    imgHandler.ControlZ();
    imgHandler.Display();

    imgHandler.ControlZ();
    imgHandler.Display();

    imgHandler.ControlY();
    imgHandler.Display();

    imgHandler.ControlY();
    imgHandler.Display();

    imgHandler.ControlY();
    imgHandler.Display();

    imgHandler.ControlY();
    imgHandler.Display();
    
    imgHandler.ControlY();
    imgHandler.Display();

    // Rotate the image by 45 degrees around the center
    imgHandler.Rotate(45, centerPoints);
    imgHandler.Display();

    imgHandler.ControlZ();
    imgHandler.Display();

	PanoramaCreator myPano(folderPath);
	Image panorama = myPano.CreatePanorama(myPano.getListImages());
	panorama.Display();

	return 0;
}
