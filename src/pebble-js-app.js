Pebble.addEventListener("ready",
  function(e) {
    console.log("PebbleKit JS ready!");
  }
);

Pebble.addEventListener("showConfiguration",
  function(e) {
    //Load the remote config page
    Pebble.openURL("http://clach04.github.io/pebconfig/pebble-config.html");
  }
);

Pebble.addEventListener("webviewclosed",
  function(e) {
    //Get JSON dictionary
    var configuration = JSON.parse(decodeURIComponent(e.response));
    console.log("Configuration window returned: " + JSON.stringify(configuration));

    // Create a dict to send to pebble
    // new one to avoid possibilty of sending larger dict from web page
    // also allows sanity checks/conversions if needed
    var config_for_pebble = {
          "KEY_BGMIN": parseInt(configuration.bgmin, 10),
          "KEY_FGMIN": parseInt(configuration.fgmin, 10),
          "KEY_BGHOUR": parseInt(configuration.bghour, 10),
          "KEY_FGHOUR": parseInt(configuration.fghour, 10),
          "KEY_RIM": parseInt(configuration.rimface, 10)
      };
    console.log("Configuration for pebble: " + JSON.stringify(config_for_pebble));

    //Send to Pebble, persist there     
    Pebble.sendAppMessage(
      config_for_pebble,
      function(e) {
        console.log("Sending settings data...");
      },
      function(e) {
        console.log("Settings feedback failed!");
      }
    );
  }
);
