/**
 * loom/data.hpp
 * 
 * Holds a sample configuration that can be loaded
 * Temporary, until we can read from file
 * 
 * @author Alex Schlieck
 * @date 2023-04-07
*/


char config[] = "{\"interfaces\":["
"{\"class\":\"SerialInterface\", \"id\":\"serial\", \"msg\":\"Serial running\"}"
"], \"outputs\":["
"{\"id\":\"tx\", \"interface\":\"serial\"}"
"], \"inputs\":["
"{\"id\":\"rx\", \"interface\":\"serial\", \"outputs\":[\"tx\"]}"
"]}";
