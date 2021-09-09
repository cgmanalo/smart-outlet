//var host = "futurehome.hopto.org";
//var host = "192.168.254.126";
var host = "192.168.0.6";
var port = "8765";
var ws;
var ACUAState;
var ACUBState;

        onMessage = function (evt){
            var received_msg = evt.data;
            document.getElementById("message").textContent = received_msg;
            var app_name = ParseString(received_msg,1);
            var device_name = ParseString(received_msg,2);
            var device1 = ParseString(received_msg,3);
            var device2 = ParseString(received_msg,4);

            if (app_name === "SWITCH"){
                if (device_name === "ALL"){
                    if (device1 === "ON"){
                        document.getElementById("masterlight").style.backgroundColor = "lightgreen";
                    }
                    else{
                        document.getElementById("masterlight").style.backgroundColor = "darkgreen";
                    }
                    if (device2 === "ON"){
                        document.getElementById("light").style.backgroundColor = "lightgreen";
                    }
                    else{
                        document.getElementById("light").style.backgroundColor = "darkgreen";
                    }
                    document.getElementById("temp").textContent = ParseString(received_msg,5) + " 'C";
                }
            }
            if (app_name === "OUTLET"){
                if (device_name === "ALL"){
                    if (device1 === "ON"){
                        document.getElementById("masterfan").style.backgroundColor = "lightgreen";
                    }
                    else{
                        document.getElementById("masterfan").style.backgroundColor = "darkgreen";
                    }
                    if (device2 === "ON"){
                        document.getElementById("mastertv").style.backgroundColor = "lightgreen";
                    }
                    else{
                        document.getElementById("mastertv").style.backgroundColor = "darkgreen";
                    }
                    document.getElementById("temp").textContent = ParseString(received_msg,5) + " 'C";
                }
            }
            if (app_name === "LVG_SW"){
                if (device_name === "ALL"){
                    if (device1 === "ON"){
                        document.getElementById("dininglight").style.backgroundColor = "lightgreen";
                    }
                    else{
                        document.getElementById("dininglight").style.backgroundColor = "darkgreen";
                    }
                    document.getElementById("temp").textContent = ParseString(received_msg,5) + " 'C";
                }

            }
            if (app_name === "CHIM"){
                if (device_name === "ACUA"){
                    if (device1 === "ON"){
                        ACUAState = "ON";
                        document.getElementById("acu").style.backgroundColor = "lightgreen";
                    }
                    else{
                        ACUAState = "OFF";
                        document.getElementById("acu").style.backgroundColor = "darkgreen";
                    }
                    document.getElementById("temp").textContent = ParseString(received_msg,10) + " 'C";
                    document.getElementById("acustat").textContent = ParseString(received_msg,4) + " V, " + ParseString(received_msg,5) + " A, " + ParseString(received_msg,8) + " kWHr";
                }
                else if (device_name === "ACUB"){
                    if (device1 === "ON"){
                        ACUBState = "ON";
                        document.getElementById("joshua_acu").style.backgroundColor = "lightgreen";
                    }
                    else{
                        ACUBState = "OFF";
                        document.getElementById("joshua_acu").style.backgroundColor = "darkgreen";
                    }
                    document.getElementById("temp").textContent = ParseString(received_msg,10) + " 'C";
                    document.getElementById("joshua_acustat").textContent = ParseString(received_msg,4) + " V, " + ParseString(received_msg,5) + " A, " + ParseString(received_msg,8) + " kWHr";
                }

            }
            else if (app_name === "DNG_PWR"){
                if (device_name === "ALL"){
                    if (device1 === "ON"){
                        document.getElementById("fan2").style.backgroundColor = "lightgreen";
                    }
                    else{
                        document.getElementById("fan2").style.backgroundColor = "darkgreen";
                    }
                    if (device2 === "ON"){
                        document.getElementById("fan").style.backgroundColor = "lightgreen";
                    }
                    else{
                        document.getElementById("fan").style.backgroundColor = "darkgreen";
                    }

                    document.getElementById("temp").textContent = document.getElementById("temp").textContent = ParseString(received_msg,5) + " 'C";
                }
            }
            else if (app_name === "TIMER"){
                if (parseInt(device_name) > 0){
                    document.getElementById("message").textContent = "Connection will terminate in " + device_name + " s";
                }
                else{
                    document.getElementById("message").textContent = "Connection terminated";
                }
            }
            else if (app_name === "WAIT"){
                document.getElementById("message").textContent = "Waiting for reply...";
            }
            else if (app_name === "USERS"){
                document.getElementById("users").textContent = "Connected users: " + device_name;
            }
            else if (app_name === "ACTIVEJOBS"){
                document.getElementById("message").textContent = ParseString(received_msg,2) + " jobs active.";
                document.getElementById("temp").textContent = document.getElementById("temp").textContent = ParseString(received_msg,3) + " 'C";
            }
            else if (app_name === "JOB10"){
                if (ParseString(received_msg,9) === "Checked"){
                    var jobtime = ParseString(received_msg,5) + ":" + ParseString(received_msg,6) + " " + ParseString(received_msg,7);
                    document.getElementById("message2").textContent = ParseString(received_msg,3) + " turn " + ParseString(received_msg,4) + " time: " + jobtime;
                }
                else{
                    document.getElementById("message2").textContent = "ACUA turn OFF time: Unlimited";
                }
            }
        };

        onOpen = function (){
            document.getElementById("connectmessage").textContent = "Connected to server";
            document.getElementById("message").textContent = "Connected to server " +  host + ":" + port + ".";
            document.getElementById("sconnect").value = "Disconnect";
            document.getElementById("sconnect").style.backgroundColor = "lightgreen";
        };

        onClose = function (){
            document.getElementById("connectmessage").textContent = "Disconnected from server.";
            document.getElementById("message").textContent = "Connection terminated.";
            document.getElementById("sconnect").value = "Connect";
            document.getElementById("sconnect").style.backgroundColor = "red";
            document.getElementById("users").textContent = "Connected users: ";
        };

        onOpenTest = function (){
            alert("test open");
            ws.send("DNG_PWR|OUTLET2|TOG");
            
        }

        onMessageTest = function (evt){
            var received_msg = evt.data;
            //alert(received_msg);
            ws.onmessage = onMessage;
        }

        function Connect (){
            try{
                if (document.getElementById("sconnect").value === "Connect"){
                    //var url = "wss://" + host + ":" + port + "/";
                    var url = "ws://" + host + ":" + port + "/";
                    ws = new WebSocket(url);
                    //ws = new WebSocket(url, {
                    //    protocolVersion:8,
                    //    origin: "https://" + host + ":" + port,
                    //    rejectUnauthorized: false
                    //});
                    document.getElementById("message").textContent = "Connecting to server " +  host + ":" + port + "...";
                    ws.onopen = onOpen;
                    ws.onmessage = onMessage;
                    ws.onclose = onClose;
                    //ws.onmessage = onMessageTest;
                    //ws.onopen = onOpenTest;


                }
                else {
                    ws.send("Disconnect");
                }
            }
            catch (err){
                alert("Cannot connect to server.");
            }
        }


        function Page2 (){
            var newwin = window.open("page2.html");

            newwin.onload = function (){
                this.port = port;
                this.ws = ws;
            };

        }

        function JoshuaStatus (){
            try{
                if (ws.readyState){
                    ws.send("CHIM|ACUB");
                }
                else{
                    alert("You're not connected!")
                }
            }
            catch (err){
                document.getElementById("message").textContent = "It seems you're not connected.";
            }

        }

        function JoshuaShutdown (){
            try{
                if (ws.readyState){
                    ws.send("CHIM|ACUB|OFF");
                }
                else{
                    alert("You're not connected!")
                }
            }
            catch (err){
                document.getElementById("message").textContent = "It seems you're not connected.";
            }

        }

        function JoshuaACU (){
            try{
                if (ws.readyState){
                    ACUBState = "";
		    checkACU_ON_Period(30, 8, 7, "CHIM|ACUB|OFF", "");
                    ws.send("CHIM|ACUB|TOG");
                    setTimeout(() => { 
                        if (ACUBState === "ON" || ACUBState === ""){ 
                            checkACU_ON_Period("unlimited", 8, 7, "CHIM|ACUB|OFF", "");
                        }
                        else if (ACUBState === "OFF"){
                            jobRequest = "SubmitJob8||||||||Unchecked|||";
                            ws.send(jobRequest);
                            jobRequest = "SubmitJob7||||||||Unchecked|||";
                            ws.send(jobRequest);
                        }
                    }, 20000);

                }
                else{
                    alert("You're not connected!")
                }
            }
            catch (err){
                document.getElementById("message").textContent = "It seems you're not connected.";
            }

        }
        function JoshuaACUStatus (){
            try{
                if (ws.readyState){
                    ws.send("CHIM|ACUB");
                }
                else{
                    alert("You're not connected!")
                }
            }
            catch (err){
                document.getElementById("message").textContent = "It seems you're not connected.";
            }

        }


        function Disconnect (){
            ws.send("Disconnect");
        }

        function Light (){
            try{
                if (ws.readyState){
                    ws.send("SWITCH|SWITCH2|TOG");
                }
                else{
                    alert("You're not connected!")
                }
            }
            catch (err){
                document.getElementById("message").textContent = "It seems you're not connected.";
            }
        }
        function Fan (){
            try{
                if (ws.readyState){
                    ws.send("DNG_PWR|OUTLET2|TOG");
                }
                else{
                    alert("You're not connected!")
                }
            }
            catch (err){
                document.getElementById("message").textContent = "It seems you're not connected.";
            }

        }
        function Fan2 (){
            try{
                if (ws.readyState){
                    ws.send("DNG_PWR|OUTLET1|TOG");
                }
                else{
                    alert("You're not connected!")
                }
            }
            catch (err){
                document.getElementById("message").textContent = "It seems you're not connected.";
            }

        }

        function LightStatus (){
            try{
                if (ws.readyState){
                    ws.send("SWITCH|SWITCH1");
                }
                else{
                    alert("You're not connected!")
                }
            }
            catch (err){
                document.getElementById("message").textContent = "It seems you're not connected.";
            }

        }
        function DiningLight (){
            try{
                if (ws.readyState){
                    ws.send("LVG_SW|SWITCH1|TOG");
                }
                else{
                    alert("You're not connected!")
                }
            }
            catch (err){
                document.getElementById("message").textContent = "It seems you're not connected.";
            }

        }

        function DiningLightStatus (){
            try{
                if (ws.readyState){
                    ws.send("LVG_SW|SWITCH1");
                }
                else{
                    alert("You're not connected!")
                }
            }
            catch (err){
                document.getElementById("message").textContent = "It seems you're not connected.";
            }

        }

        function LaboratoryStatus (){
            try{
                if (ws.readyState){
                    ws.send("SWITCH|SWITCH1");
                    ws.send("DNG_PWR|OUTLET2");                
                }
                else{
                    alert("You're not connected!")
                }
            }
            catch (err){
                document.getElementById("message").textContent = "It seems you're not connected.";
            }

        }
        function DiningStatus (){
            try{
                if (ws.readyState){
                    ws.send("LVG_SW|SWITCH1");
                }
                else{
                    alert("You're not connected!")
                }
            }
            catch (err){
                document.getElementById("message").textContent = "It seems you're not connected.";
            }

        }

        function MasterStatus (){
            try{
                if (ws.readyState){
                    ws.send("SWITCH|SWITCH1");
                    ws.send("OUTLET|OUTLET1");
                    ws.send("OUTLET|OUTLET1");
                    ws.send("CHIM|ACUA");
                }
                else{
                    alert("You're not connected!")
                }
            }
            catch (err){
                document.getElementById("message").textContent = "It seems you're not connected.";
            }

        }

        function FanStatus (){
            try{
                if (ws.readyState){
                    ws.send("DNG_PWR|OUTLET2");
                }
                else{
                    alert("You're not connected!")
                }
            }
            catch (err){
                document.getElementById("message").textContent = "It seems you're not connected.";
            }

        }
        function Fan2Status (){
            try{
                if (ws.readyState){
                    ws.send("DNG_PWR|OUTLET1");
                }
                else{
                    alert("You're not connected!")
                }
            }
            catch (err){
                document.getElementById("message").textContent = "It seems you're not connected.";
            }

        }

        function MasterLight (){
            try{
                if (ws.readyState){
                    ws.send("SWITCH|SWITCH1|TOG");
                }
                else{
                    alert("You're not connected!")
                }
            }
            catch (err){
                document.getElementById("message").textContent = "It seems you're not connected.";
            }
        }
        function MasterFan (){
            try{
                if (ws.readyState){
                    ws.send("OUTLET|OUTLET1|TOG");
                }
                else{
                    alert("You're not connected!")
                }
            }
            catch (err){
                document.getElementById("message").textContent = "It seems you're not connected.";
            }

        }
        function MasterTV (){
            try{
                if (ws.readyState){
                    ws.send("OUTLET|OUTLET2|TOG");
                }
                else{
                    alert("You're not connected!")
                }
            }
            catch (err){
                document.getElementById("message").textContent = "It seems you're not connected.";
            }

        }
        function ACU (){
            try{
                if (ws.readyState){
                    var OnPeriod = document.getElementById("acuduration").value;
                    ACUAState = "";
	            checkACU_ON_Period(OnPeriod, 10, 9, "CHIM|ACUA|OFF", "");
                    ws.send("CHIM|ACUA|TOG");
                    setTimeout(() => { 
                        if (ACUAState === "ON" || ACUAState === ""){                          
                            checkACU_ON_Period(OnPeriod, 10, 9, "CHIM|ACUA|OFF", "OUTLET|OUTLET1|ON");
                        }
                        else if (ACUAState === "OFF"){
                            jobRequest = "SubmitJob10||||||||Unchecked|||";
                            ws.send(jobRequest);
                            jobRequest = "SubmitJob9||||||||Unchecked|||";
                            ws.send(jobRequest);
                        }
                    }, 20000);
                }
                else{
                    alert("You're not connected!")
                }
            }
            catch (err){
                document.getElementById("message").textContent = "It seems you're not connected.";
            }

        }

        function Reload (){
            if (ACUAState === "ON"){
                var OnPeriod = document.getElementById("acuduration").value;
                checkACU_ON_Period(OnPeriod, 10, 9, "CHIM|ACUA|OFF", "OUTLET|OUTLET1|ON");
            }
            else{
                alert("ACU is OFF!");
            }
        }

        function checkACU_ON_Period (OnPeriod, ACUJobNo, FanJobNo, ACUStringCommand, FanStringCommand){
            var totalMinutes = OnPeriod;
            var jobRequest;
            var futureDateTime;
            var futureDateTime2;
            var futureHour;
            var futureMinute;
            var AMPM;
            if (totalMinutes === "unlimited"){
                jobRequest = "SubmitJob" + ACUJobNo + "||||||||Unchecked|||";
                ws.send(jobRequest);
            }
            else{
                futureDateTime = new Date();
                futureDateTime2 = futureDateTime;
                futureDateTime.setMinutes(futureDateTime.getMinutes() + parseInt(totalMinutes));
                futureHour = futureDateTime.getHours();
                futureMinute = futureDateTime.getMinutes();
                AMPM = "AM";
                if (futureHour === 0 && futureMinute < 5){
                    futureMinute = 5;
                }
                if (futureHour === 23 && futureMinute > 55){
                    futureMinute = 55;
                }
                if (futureHour > 11){
                    AMPM = "PM";
                    if (futureHour > 12){
                        futureHour = futureHour - 12;
                    }
                }
                if (futureHour < 10){
                    futureHour = "0" + futureHour.toString();
                }
                else{
                    futureHour = futureHour.toString();
                }
                if (futureMinute < 10){
                    futureMinute = "0" + futureMinute.toString();
                }
                else{
                    futureMinute = futureMinute.toString();
                }
                jobRequest = "SubmitJob" + ACUJobNo + "|"  + ACUStringCommand + "|" + futureHour + "|" + futureMinute + "|" + AMPM + "|3|Checked|Waiting|01/01 00:00|Unchecked";
                ws.send(jobRequest);
                if (document.getElementById("on_fan").checked){
                    futureDateTime2.setMinutes(futureDateTime2.getMinutes() - 1);
                    futureHour = futureDateTime2.getHours();
                    futureMinute = futureDateTime2.getMinutes();
                    AMPM = "AM";
                    if (futureHour === 0 && futureMinute < 5){
                        futureMinute = 4;
                    }
                    if (futureHour === 23 && futureMinute > 55){
                        futureMinute = 54;
                    }
                    if (futureHour > 11){
                        AMPM = "PM";
                        if (futureHour > 12){
                            futureHour = futureHour - 12;
                        }
                    }
                    if (futureHour < 10){
                        futureHour = "0" + futureHour.toString();
                    }
                    else{
                        futureHour = futureHour.toString();
                    }
                    if (futureMinute < 10){
                        futureMinute = "0" + futureMinute.toString();
                    }
                    else{
                        futureMinute = futureMinute.toString();
                    }
                    //jobRequest = "SubmitJob9|OUTLET|OUTLET1|ON|" + futureHour + "|" + futureMinute + "|" + AMPM + "|3|Checked|Waiting|01/01 00:00|Unchecked";
                    jobRequest = "SubmitJob" + FanJobNo + "|" + FanStringCommand + "|" + futureHour + "|" + futureMinute + "|" + AMPM + "|3|Checked|Waiting|01/01 00:00|Unchecked";                   
                    ws.send(jobRequest);
                }
                else{
                    jobRequest = "SubmitJob" + FanJobNo + "||||||||Unchecked|||";
                    ws.send(jobRequest);
                }

            }
        }

        function MasterLightStatus (){
            try{
                if (ws.readyState){
                    ws.send("SWITCH|SWITCH1");
                }
                else{
                    alert("You're not connected!")
                }
            }
            catch (err){
                document.getElementById("message").textContent = "It seems you're not connected.";
            }

        }
        function MasterFanStatus (){
            try{
                if (ws.readyState){
                    ws.send("OUTLET|OUTLET1");
                }
                else{
                    alert("You're not connected!")
                }
            }
            catch (err){
                document.getElementById("message").textContent = "It seems you're not connected.";
            }

        }
        function MasterTVStatus (){
            try{
                if (ws.readyState){
                    ws.send("OUTLET|OUTLET1");
                }
                else{
                    alert("You're not connected!")
                }
            }
            catch (err){
                document.getElementById("message").textContent = "It seems you're not connected.";
            }

        }
        function ACUStatus (){
            try{
                if (ws.readyState){
                    ws.send("CHIM|ACUA");
                    ws.send("GetJob10");
                }
                else{
                    alert("You're not connected!")
                }
            }
            catch (err){
                document.getElementById("message").textContent = "It seems you're not connected.";
            }

        }

        function MasterShutdown (){
            try{
                if (ws.readyState){
                    ws.send("SWITCH|SWITCH1|OFF");
                    ws.send("OUTLET|OUTLET1|OFF");
                    ws.send("OUTLET|OUTLET2|OFF");
                    ws.send("CHIM|ACUA|OFF");
                }
                else{
                    alert("You're not connected!")
                }
            }
            catch (err){
                document.getElementById("message").textContent = "It seems you're not connected.";
            }

        }

        function LaboratoryShutdown (){
            try{
                if (ws.readyState){
                    ws.send("SWITCH|SWITCH2|OFF");
                    ws.send("DNG_PWR|OUTLET2|OFF");
                }
                else{
                    alert("You're not connected!")
                }
            }
            catch (err){
                document.getElementById("message").textContent = "It seems you're not connected.";
            }

        }

        function RestartServer (){
            try{
                if (ws.readyState){
                    ws.send("Restart");
                }
                else{
                    alert("You're not connected!")
                }
            }
            catch (err){
                document.getElementById("message").textContent = "It seems you're not connected.";
            }
        }

        function SendCommand (){
            try{
                if (ws.readyState){
                    var command = document.getElementById("command").value;
                    ws.send(command);
                }
                else{
                    alert("You're not connected!")
                }
            }
            catch (err){
                document.getElementById("message").textContent = "It seems you're not connected.";
            }

        }

       
        function CheckJobs (){
            try{
                if (ws.readyState){
                    ws.send("CheckJob");
                }
                else{
                    alert("You're not connected!")
                }
            }
            catch (err){
                document.getElementById("message").textContent = "It seems you're not connected.";
            }

        }

        function ChangeColorOn (){
            document.getElementById("sconnect").style.backgroundColor="blue";
        }

        function ChangeColorOff (){
            document.getElementById("sconnect").style.backgroundColor="lightgrey";
        }



        function json(url) {
            return fetch(url).then(res => res.json());
        }
        set_event_routines = function (){
            ws.onopen = onOpen;
            ws.onmessage = onMessage;
            ws.onclose = onClose;
        }

        ParseString = function (rawString, pos){
            return rawString.split("|")[pos-1];
        }

        function DisplayIP(response) {
            document.getElementById("ipaddress").innerHTML = "Your IP Address is " + response.ip;
        }
	setInterval(myTimer, 1000);

	function myTimer() {
  	    const d = new Date();
  	    document.getElementById("date-time").innerHTML = d.toLocaleTimeString();
	}