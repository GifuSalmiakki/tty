import logo from './logo.svg';
import './App.css';
import React from 'react';
import CombinationLock from 'combination-lock-react'
import { useState } from 'react';
import reactToString from 'react-to-string'

const lockOpened = () => {

  alert("Unlocked!")
}


function App() {

  var [numbercode, setCode] = useState("01234")

  return (
    <div className="App">

        <h2>Exercise 2.2</h2>
        <p> Default code 01234</p>
        <img src={logo} className="App-logo" alt="logo" />
        
        <CombinationLock
        combination={reactToString(numbercode)}
        height={80}
        onMatch={lockOpened} 
        />
        
        <br></br>

        <label> Change combination
        <input type="password"
        value={numbercode}
        onChange={(e) => setCode(e.target.value)}
        />
        </label>


    </div>
  );
}

export default App;
