#include "ofApp.h"
#include "ofxPS3EyeGrabber.h"


void ofApp::setup()
{ 
    // Set the custom ofxPS3EyeGrabber.
    grabber.setGrabber(std::make_shared<ofxPS3EyeGrabber>());

    // The native pixel format for the ofxPS3EyeGrabber is a Bayer pattern
    // (it will look black and white).
    grabber.setPixelFormat(OF_PIXELS_NATIVE);
    grabber.setDesiredFrameRate(60);
    grabber.setup(640, 480);

    // These are examples of ofxPS3EyeGrabber-specific paramaters.
    // must be set after the grabber is set up.
    grabber.getGrabber<ofxPS3EyeGrabber>()->setAutogain(false);
    grabber.getGrabber<ofxPS3EyeGrabber>()->setAutoWhiteBalance(false);
    grabber.getGrabber<ofxPS3EyeGrabber>()->setFlipHorizontal(false);
    grabber.getGrabber<ofxPS3EyeGrabber>()->setFlipVertical(false);

    cout << "res: " << grabber.getGrabber<ofxPS3EyeGrabber>()->getWidth() << " x " << grabber.getGrabber<ofxPS3EyeGrabber>()->getHeight() << endl;
    cout << "flip h: " << grabber.getGrabber<ofxPS3EyeGrabber>()->getFlipHorizontal() << endl;
    cout << "flip v: " << grabber.getGrabber<ofxPS3EyeGrabber>()->getFlipVertical() << endl;

    imgDebayer.allocate(grabber.getWidth(), grabber.getHeight(), OF_IMAGE_GRAYSCALE);

    // parameters
    parameters.setName("ps3 eye");
    parameters.add( gain.set("gain", 20, 0, 63));
    parameters.add( exposure.set("exposure", 120, 0, 255));
    //parameters.add( sharpness.set("sharpness", 0, 0, 255));
    parameters.add( hue.set("hue", 143, 0, 255));
    //parameters.add( brightness.set("brightness", 20, 0, 255));
    //parameters.add( contrast.set("contrast", 37, 0, 255));
    parameters.add( redBalance.set("red balance", 128, 0, 255));
    parameters.add( greenBalance.set("green balance", 128, 0, 255));
    parameters.add( blueBalance.set("blue balance", 128, 0, 255));
    parameters.add( bTestPattern.set("testPattern", false));    
    parameters.add( bDebayer.set("demosaicing", false));

    applyParameters();

    ofAddListener(parameters.parameterChangedE(), this, &ofApp::onParameterChange);

    // gui
    gui.setup();
    gui.add(parameters);
}


void ofApp::update()
{
    grabber.update();

    if (bDebayer) 
    {
        if (grabber.isFrameNew())
        {
            auto psEye = grabber.getGrabber<ofxPS3EyeGrabber>();
            imgDebayer.setFromPixels(psEye->bayerConverter(psEye->getPixels(), OF_PIXELS_GRAY, false, ofxPS3EyeGrabber::DemosaicType::DEMOSAIC_BILINEAR));
            imgDebayer.update();
        }
    }
}


void ofApp::draw()
{
    ofBackground(0);
    ofSetColor(255);

    grabber.draw(0, 0);
    
    if (bDebayer) imgDebayer.draw(640, 0);

    std::stringstream ss;
    ss << " App FPS: " << ofToString(ofGetFrameRate(), 0) << std::endl;
    ss << " Cam FPS: " << ofToString(grabber.getGrabber<ofxPS3EyeGrabber>()->getFPS(), 0) << std::endl;
    ss << "Real FPS: " << ofToString(grabber.getGrabber<ofxPS3EyeGrabber>()->getActualFPS(), 0);
    //ss << "      id: 0x" << ofToHex(grabber.getGrabber<ofxPS3EyeGrabber>()->getDeviceId());
    ofDrawBitmapStringHighlight(ss.str(), ofPoint(ofGetWidth() - 100, 15));

    gui.draw();
}


void ofApp::applyParameters()
{
    auto psEye = grabber.getGrabber<ofxPS3EyeGrabber>();
    
    psEye->setGain(gain);
    psEye->setExposure(exposure);
    psEye->setSharpness(sharpness);
    psEye->setHue(hue);
    psEye->setBrightness(brightness);
    psEye->setContrast(contrast);
    psEye->setRedBalance(redBalance);
    psEye->setGreenBalance(greenBalance);
    psEye->setBlueBalance(blueBalance);
    psEye->setTestPattern(bTestPattern);
}


void ofApp::onParameterChange(ofAbstractParameter& p)
{
    auto psEye = grabber.getGrabber<ofxPS3EyeGrabber>();

    if (p.getName() == "gain")               psEye->setGain(p.cast<int>().get());
    else if (p.getName() == "exposure")      psEye->setExposure(p.cast<int>().get());
    else if (p.getName() == "sharpness")     psEye->setSharpness(p.cast<int>().get());
    else if (p.getName() == "hue")           psEye->setHue(p.cast<int>().get());
    else if (p.getName() == "brightness")    psEye->setBrightness(p.cast<int>().get());
    else if (p.getName() == "contrast")      psEye->setContrast(p.cast<int>().get());
    else if (p.getName() == "red balance")   psEye->setRedBalance(p.cast<int>().get());
    else if (p.getName() == "green balance") psEye->setGreenBalance(p.cast<int>().get());
    else if (p.getName() == "blue balance")  psEye->setBlueBalance(p.cast<int>().get());
    else if (p.getName() == "testPattern")   psEye->setTestPattern(p.cast<bool>().get());
}
