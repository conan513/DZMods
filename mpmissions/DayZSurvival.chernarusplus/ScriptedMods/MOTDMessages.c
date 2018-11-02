int TIME_BT_EACH_MSG = 60000; //in ms
int TIME_INTERVAL = TIME_BT_EACH_MSG + Math.RandomIntInclusive(5000,15000); //5 sec - 15 sec Random addition to TIME_BT_EACH_MSG
bool m_MOTD;
string line1;
string line2;
string line3;

void CustomMOTD()
{	
	if(m_MOTD)
	{
		TStringArray Messages = {line1,line2,line3};
		GetGame().ChatPlayer(5,Messages.GetRandomElement());
	}
}