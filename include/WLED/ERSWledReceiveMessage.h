 // Functionality to execute if we receive messages from ERS Server
  if (jsonDoc["rgb"] != nullptr)
  {
    String rgb = jsonDoc["rgb"];
    int pos = rgb.indexOf(',');
    String red = rgb.substring(0, pos);
    rgb = rgb.substring(pos + 1);
    pos = rgb.indexOf(',');
    String green = rgb.substring(0, pos);
    String blue = rgb.substring(pos + 1);
    glow_red = red.toInt();
    glow_green = green.toInt();
    glow_blue = blue.toInt();
  }