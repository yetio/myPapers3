#include "rotate.h"
#include "../ui.h"
#include "../screens/txt_viewer_screen.h"
#include "../screens/img_viewer_screen.h"

static int currentImgRotation = 0;
static int currentTxtRotation = 0;

void rotateImg180Action() {
    currentImgRotation = (currentImgRotation + 180) % 360;
    

    String currentFile = screens::getCurrentImgFile();
    if (currentFile.length() > 0) {

        M5.Display.setRotation(currentImgRotation / 90);
        

        M5.Display.fillScreen(TFT_WHITE);
        screens::drawImgViewerScreen(currentFile);
    }
}

void rotateTxt180Action() {
    currentTxtRotation = (currentTxtRotation + 180) % 360;
    

    String currentFile = screens::getCurrentTxtFile();
    if (currentFile.length() > 0) {

        M5.Display.setRotation(currentTxtRotation / 90);
        

        M5.Display.fillScreen(TFT_WHITE);
        screens::drawTxtViewerScreen(currentFile);
    }
}