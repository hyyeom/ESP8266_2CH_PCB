// Get current sensor readings when the page loads
window.addEventListener('load', getReadings());
// Function to get current readings on the web page when it loads for the first time

function getReadings() {
	var xhr = new XMLHttpRequest();
	xhr.onreadystatechange = function() {
		if (this.readyState == 4 && this.status == 200) {
			var myObj = JSON.parse(this.responseText);
			console.log(myObj);
			document.getElementById("temp").innerHTML = myObj.temperature;
			document.getElementById("hum").innerHTML = myObj.humidity;
			document.getElementById("sw1state").innerHTML = myObj.sw1state;
			document.getElementById("sw2state").innerHTML = myObj.sw2state;
			if(parseInt(myObj.key) != "6227")
				window.location = "index.html"; // Redirecting to other page.
		};
	}
	xhr.open("GET", "/readings", true);
	xhr.send();
}
// Create an Event Source to listen for events
if (!!window.EventSource) {
	var source = new EventSource('/events');
	
	source.addEventListener('open', function(e) {
		console.log("Events Connected");
	}, false);
	
	source.addEventListener('error', function(e) {
		if (e.target.readyState != EventSource.OPEN) {
			console.log("Events Disconnected");
		}
	}, false);
	
	source.addEventListener('new_readings', function(e) {
		console.log("new_readings", e.data);
		var obj = JSON.parse(e.data);
		document.getElementById("temp").innerHTML = myObj.temperature;
			document.getElementById("hum").innerHTML = myObj.humidity;
			document.getElementById("sw1state").innerHTML = myObj.sw1state;
			document.getElementById("sw2state").innerHTML = myObj.sw2state;
	}, false);
}
function Sw1click() {
	var xhr = new XMLHttpRequest();
	xhr.onreadystatechange = function() {
		if (this.readyState == 4 && this.status == 200) {
			var myObj = JSON.parse(this.responseText);
			console.log(myObj);
			document.getElementById("temp").innerHTML = myObj.temperature;
			document.getElementById("hum").innerHTML = myObj.humidity;
			document.getElementById("sw1state").innerHTML = myObj.sw1state;
			document.getElementById("sw2state").innerHTML = myObj.sw2state;
		}
	};
	xhr.open("GET", "/onoffSw1", true);
	xhr.send();
}
function Sw2click() {
	var xhr = new XMLHttpRequest();
	xhr.onreadystatechange = function() {
		if (this.readyState == 4 && this.status == 200) {
			var myObj = JSON.parse(this.responseText);
			console.log(myObj);
			document.getElementById("temp").innerHTML = myObj.temperature;
			document.getElementById("hum").innerHTML = myObj.humidity;
			document.getElementById("sw1state").innerHTML = myObj.sw1state;
			document.getElementById("sw2state").innerHTML = myObj.sw2state;
		}
	};
	xhr.open("GET", "/onoffSw2", true);
	xhr.send();
}
function readData() {
	var xhr = new XMLHttpRequest();
	xhr.onreadystatechange = function() {
		if (this.readyState == 4 && this.status == 200) {
			var myObj = JSON.parse(this.responseText);
			console.log(myObj);
			document.getElementById("temp").innerHTML = myObj.temperature;
			document.getElementById("hum").innerHTML = myObj.humidity;
			document.getElementById("sw1state").innerHTML = myObj.sw1state;
			document.getElementById("sw2state").innerHTML = myObj.sw2state;
		}
	};
	xhr.open("GET", "/readData", true);
	xhr.send();
}
