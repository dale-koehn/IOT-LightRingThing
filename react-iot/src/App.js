import React, { useState, useEffect  } from 'react';
import axios from 'axios';
import './App.css';

/**
 * This app is for interfacing with AWS IOT things.
 * Currently i'm just playing with the esp32 I have publishing and subscribing to mqtt topics.
 * 
 * Right now this app can publish messages to a lambda that publishes to an mqtt topic.
 * I don't have a way of seeing what comes out of the esp32 here (yet)
 * 
 */

// If a json is published to this url it gets published to mqtt topic esp32/sub
const PUBLISH_MESSAGE_URL = "https://dale.lefthandedcatstudio.com/iot/publish-mqtt";

// Horizontal % offsets for buttons to roughly fit a circle
// This only defines half of the circle ... for button indices 0 to 7
const BUTTON_POSITIONS = [ 20, 56, 83, 98, 98, 83, 56, 20 ];

const NUM_BUTTONS=16;

const ON_COLOR = '#f48787';

let BUTTONS = [];
for(let i=0; i<NUM_BUTTONS; ++i) {
  BUTTONS[i] = false;
}

function publishTestMessage() {
  axios.post( PUBLISH_MESSAGE_URL, {foo: 'bar' } );
}

function publishButtons(buttons) {
  let msg = { buttons: buttons };
  axios.post( PUBLISH_MESSAGE_URL, msg );
}

function toggleButton(i, buttons, setButtons) {
  let b = [ ... buttons ];
  b[i] = !b[i];
  console.log( JSON.stringify(b) );
  publishButtons(b);
  setButtons(b);
}

function allOn(setButtons) { 
  let b = [];
  for(let i=0; i<NUM_BUTTONS; ++i) {
    b[i] = true;
  }

  publishButtons(b);
  setButtons(b);
}

function allOff(setButtons) { 
  let b = [];
  for(let i=0; i<NUM_BUTTONS; ++i) {
    b[i] = false;
  }

  publishButtons(b);
  setButtons(b);
}


function App() {
  const [buttons, setButtons] = useState( [ ...BUTTONS ] );

  useEffect( () => {
    //getImages( setImages );
  }, []);

  // Would be nice to have the esp32 periodically publish its voltage and such
  // Of course i'd need to determine how to see that in this ui

  return (
    <div className="App">
      <div className="header">IOT ESP32 Dashboard</div>

      <div className="buttons" style={{marginBottom: '10px'}}>
        {[0, 1, 2, 3, 4, 5, 6, 7].map( function(v) {
          return(
            <div style={{position: 'static', height: '40px'}}>
              <button id={v} onClick={() => toggleButton(v, buttons, setButtons)} style={{backgroundColor: buttons[v]?ON_COLOR:'', position: 'relative', left: `${BUTTON_POSITIONS[v]/2}%`}}>{buttons[v]?"On":"Off"}</button>
              <button id={15-v} onClick={() => toggleButton(15-v, buttons, setButtons)} style={{backgroundColor: buttons[15-v]?ON_COLOR:'', position: 'relative', left: `${-BUTTON_POSITIONS[v]/2}%`}}>{buttons[15-v]?"On":"Off"}</button>
            </div>
          )
        })}
      </div>

      <button onClick={ () => allOn(setButtons) }>All On</button>
      <button onClick={ () => allOff(setButtons) }>All Off</button>

    </div>
  );
}

export default App;
