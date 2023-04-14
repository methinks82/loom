/**
 * loom/data.hpp
 * 
 * Holds a sample configuration that can be loaded
 * Temporary, until we can read from file
 * 
 * @author Alex Schlieck
 * @date 2023-04-07
*/


char jsonConfig[] = 
"{\"interfaces\":["
    "{\"class\":\"SerialInterface\", \"id\":\"serial\", \"msg\":\"foo\", \"outputs\":[" 
        "{\"id\":\"tx\"}"
    "],\"inputs\":["
        "{\"id\":\"rx\", \"outputs\":[\"tx\",\"led\",\"oled\"]}"
    "]},"
    "{\"class\":\"GpioInterface\", \"id\":\"gpio\", \"outputs\":["
        "{\"id\":\"led\", \"pin\":5}"
    "],\"inputs\":["
        "{\"id\":\"btn\", \"pin\":12,\"outputs\":[\"tx\",\"led\"]}"
    "]},"
    "{\"class\":\"ScreenInterface\", \"id\":\"screen\", \"outputs\":["
        "{\"id\":\"oled\"}"
    "]}"
"]}";


char yamlConfig[] = "---\n"
"interfaces:\n"
"- class: SerialInterface"
"  id: serial"
"  outputs:"
"  - id:tx"
"  inputs:"
"  - id:rx"
"    outputs[tx]"
;

/*
char jsonConfig[] = 
"{\"interfaces\":["
    "{\"class\":\"SerialInterface\", \"id\":\"serial\", \"msg\":\"foo\", \"outputs\":[" 
        "{\"id\":\"tx\"}"
    "],\"inputs\":["
        "{\"id\":\"rx\", \"outputs\":[\"tx\"]}"
    "]}"
"]}";
*/