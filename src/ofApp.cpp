#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //ofSetEscapeQuitsApp(false);
    ofSetWindowTitle(TITLE + " v" + VERSION);
    ofSetBackgroundColor(0, 0, 0);
    ofSetVerticalSync(true);
    //ofSetFrameRate(30);
    buildGui();
    
    restoreSettings();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    showLog();
}

//--------------------------------------------------------------
void ofApp::exit(){
    saveSettings();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key==' '){
        bShowGui=!bShowGui;
        gui->setVisible(bShowGui);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::onDropdownEvent(ofxDatGuiDropdownEvent e)
{
    cout << "onDropdownEvent: " << e.target->getLabel() << " Selected" << endl;
    setMidiPort( e.target->getLabel() );
}

//--------------------------------------------------------------
void ofApp::setMidiPort(string pPortName){
    midiIn.closePort();
    midiIn.removeListener(this);
    
    midiIn.openPort( pPortName ); // open a virtual port
    //midiIn.ignoreTypes(false, // sysex  <-- don't ignore timecode messages!
    //                   false, // timing <-- don't ignore clock messages!
    //                   true); // sensing
    
    // add ofApp as a listener
    midiIn.addListener(this);
    cout << "setMidiPoprt: " << pPortName << " Selected" << endl;
}

//--------------------------------------------------------------
void ofApp::newMidiMessage(ofxMidiMessage& message) {
   //addLog("Midi INcoming:" + message.getStatusString(message.status));
   addLog("Midi INcoming:" + ofToString(message.value));
    switch(message.status) {
        case MIDI_TIME_CLOCK:
            break;
        case MIDI_START: case MIDI_CONTINUE:
            break;
        case MIDI_STOP:
            break;
        case MIDI_NOTE_ON:
            //processMidi_NoteOn(message);
            break;
        case MIDI_NOTE_OFF:
            //processMidi_NoteOff(message);
            break;
        case MIDI_CONTROL_CHANGE:
            //processMidi_ControlChange(message);
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::buildGui(){
    font.load(OF_TTF_MONO, 11);
    
    gui = new ofxDatGui( ofxDatGuiAnchor::TOP_LEFT );
    vector<string> opts = {midiIn.getInPortList()/*"option - 1", "option - 2", "option - 3", "option - 4"*/};
    cmbMidiIn = gui->addDropdown("Click hier to select MidiPort", opts);
    btnClear = gui->addButton("Click it like Beckham");


    // once the gui has been assembled, register callbacks to listen for component specific events //
    //gui->onSliderEvent(this, &ofApp::onSliderEvent);
    gui->onDropdownEvent(this, &ofApp::onDropdownEvent);

    gui->setTheme(new myCustomTheme() );
    gui->setWidth( ofGetScreenWidth() );
}

//--------------------------------------------------------------
void ofApp::addLog(string p){
    if(logText.size()>17){
        logText.erase( logText.begin() );
    }
    logText.push_back( p );
}

//--------------------------------------------------------------
void ofApp::showLog(){
    string sTmp;
    //string sOscInfo="OSC Format: NoteOn/Channel/Pitch  ControlChange/Channel/Value\n\r";
    //sOscInfo += "OSC In Port:" + ofToString(incomingPortOsc) + "   OSC Out Port:" + ofToString(outGoingPortOsc) + " (see settings.xml)";
    //font.drawString(sOscInfo, 10,btnClear->getY() + 100);
    
    for(int i=0; i<logText.size(); i++){
        sTmp += logText[i] + "\n";
    }
    font.drawString(sTmp, 10,btnClear->getY() + btnClear->getHeight() +25);
}

void ofApp::restoreSettings(){
    if (xmlSettings.loadFile("settings.xml")) {
        addLog("XML loaded");
    }else{
        addLog("Could not load xml. Reverting to default values.");
    }
    
    string sMidiInPort = xmlSettings.getValue("midiInPort", "");
    if((!sMidiInPort.empty()) && (sMidiInPort.compare(LBL_NONE)!=0) ){
        vector<string> optsMidi_In = {midiIn.getInPortList()};
        optsMidi_In.push_back(LBL_NONE);

        for(int i=0; i<optsMidi_In.size(); i++){
            if(optsMidi_In[i] == sMidiInPort){
                cmbMidiIn->select(i);
                setMidiPort(sMidiInPort);
                cmbMidiIn->setLabel(LBL_CMD_MIDI_IN + sMidiInPort);
                //bMidiIn_Active=true;
                break;
            }
        }
    }else{
        cout << "LoadFromSettings: Midi in disabled";
        cmbMidiIn->setLabel(LBL_CMD_MIDI_IN + sMidiInPort);
        //bMidiIn_Active = false;
    }
}


void ofApp::saveSettings(){
    //ofLogNotice(ofToString( cmbMidiOut->getSelected()->getName() ));
    
    if(!cmbMidiIn->getSelected()->getName().empty())
        xmlSettings.setValue("midiInPort", cmbMidiIn->getSelected()->getName() );
    xmlSettings.save();
}