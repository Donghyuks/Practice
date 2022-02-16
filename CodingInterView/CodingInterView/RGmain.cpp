
#include "RandomGacha.h"

int main()
{
	RandomGacha RG;
	RG.SetStarForce("../RandomTable/StarForce.csv");

	RG.MakeWeapon(10000);

	RG.ReinforceStarforce();

	//RG.SetRebirthFireOption("../RandomTable/RebirthFire.csv");
	//
	//RG.SetAdditionalOption("../RandomTable/AdditionalOptions.csv");
	//
	//RG.ReinforceRebirth();

	return 0;
}
