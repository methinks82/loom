/**
 * loom/data.hpp
 * 
 * Holds a sample configuration that can be loaded
 * Temporary, until we can read from file
 * 
 * @author Alex Schlieck
 * @date 2023-04-07
*/


char jsonConfig[] = "{\"interfaces\":["
"{\"class\":\"SerialInterface\", \"id\":\"serial\", \"msg\":\"Serial running\"},"
"{\"class\":\"GpioInterface\", \"id\":\"gpio\"},"
"{\"class\":\"ScreenInterface\", \"id\":\"screen\"}"
"], \"outputs\":["
"{\"id\":\"tx\", \"interface\":\"serial\"},"
"{\"id\":\"oled\", \"interface\":\"screen\"}"
"], \"inputs\":["
"{\"id\":\"rx\", \"interface\":\"serial\", \"outputs\":[\"tx\",\"oled\"]},"
"{\"id\":\"btn\", \"interface\":\"gpio\", \"pin\":12, \"outputs\":[\"tx\",\"oled\"]}"
"]}";

char yamlConfig[] = "---\n"
"interfaces:\n"
"- class: SerialInterface\n"
"  id: serial\n"
"  msg:Serial Running\n"
"- class: GpioInterface\n"
"  id: gpio\n"
"outputs:\n"
"- id:tx\n"
"  interface:serial\n"
"inputs:\n"
"- id:rx\n"
"  interface:serial\n"
"  outputs:\n"
"  - tx\n"
"- id:btn\n"
"  interface:gpio\n"
"  pin:12\n"
"  outputs:\n"
"  - tx\n"
;
