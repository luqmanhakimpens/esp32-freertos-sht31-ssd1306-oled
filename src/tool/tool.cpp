/*
 * tool.cpp
 *
 *  Created on: Aug 7, 2019
 *      Author: luqma
 */


#include "tool.h"

tool::tool()
{

}
String tool::get_mac_address(bool delimited)
{
	uint8_t baseMac[6];
	// Get MAC address for WiFi station
	//esp_read_mac(baseMac, ESP_MAC_WIFI_SOFTAP);
	esp_efuse_mac_get_default(baseMac); //default is wifi sta mac
	char baseMacChr[18] = {0};

	if(delimited)
		sprintf(baseMacChr, "%02x:%02x:%02x:%02x:%02x:%02x", baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);
	else
		sprintf(baseMacChr, "%02x%02x%02x%02x%02x%02x", baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);

	return String(baseMacChr);
}

String tool::device_name()
{
	return "ntr/" + device_id();
}

String tool::device_id()
{
	return get_mac_address(false).substring(6);
}

String tool::generate_random_string(unsigned int len)
{
	String ret_str;
	for(unsigned int i=0;i<len;++i)
	{
		uint8_t char_set = random(0,3);
		char rand_char;

		if(char_set==0)
		{
			rand_char = random('0','9');
			ret_str = String(ret_str + rand_char);
		}
		else if(char_set==1)
		{
			rand_char = random('A','Z');
			ret_str = String(ret_str + rand_char);
		}
		else if(char_set==2)
		{
			rand_char = random('a','z');
			ret_str = String(ret_str + rand_char);
		}
	}
	return ret_str;
}

String tool::parse_string(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

String tool::cli_parser(String cli_str, const String& opt)
{
	if(cli_str.indexOf(opt)<0)return "NULL";

	String ret_str;
	cli_str = cli_str.substring(cli_str.indexOf(opt));
	if(cli_str.indexOf('\'')>=0)
	{
		cli_str = cli_str.substring(cli_str.indexOf('\'')+1);
		ret_str = cli_str.substring(0,cli_str.indexOf('\''));
	}
	else
	{
		cli_str = cli_str.substring(cli_str.indexOf(' ')+1);
		ret_str = cli_str.substring(0,cli_str.indexOf(' '));
		if(ret_str.indexOf('\r')>=0)ret_str = cli_str.substring(0,cli_str.indexOf('\r'));
	}
	return ret_str;
}

String tool::verbose_print_reset_reason(RESET_REASON reason)
{
  switch ( reason)
  {
    case 1  : return ("power on reset");break;
    case 3  : return ("Software reset digital core");break;
    case 4  : return ("Legacy watch dog reset digital core");break;
    case 5  : return ("Deep Sleep reset digital core");break;
    case 6  : return ("Reset by SLC module, reset digital core");break;
    case 7  : return ("Timer Group0 Watch dog reset digital core");break;
    case 8  : return ("Timer Group1 Watch dog reset digital core");break;
    case 9  : return ("RTC Watch dog Reset digital core");break;
    case 10 : return ("Instrusion tested to reset CPU");break;
    case 11 : return ("Time Group reset CPU");break;
    case 12 : return ("Software reset CPU");break;
    case 13 : return ("RTC Watch dog Reset CPU");break;
    case 14 : return ("for APP CPU, reseted by PRO CPU");break;
    case 15 : return ("Reset when the vdd voltage is not stable");break;
    case 16 : return ("RTC Watch dog reset digital core and rtc module");break;
    default : return ("NO_MEAN");
  }
}

void tool::md5_hash(const char *input, char *output)
{
	MD5Builder md5;
	md5.begin();
	md5.add(input);
	md5.calculate();
	md5.getChars(output);
}

bool tool::is_alphanumeric(const char* str)
{
	size_t len = strlen(str);
	for(int i = 0; i< len; i++)
	{
		if(!std::isalnum(str[i]))
		{
			return false;
		}
	}
	return true;
}
