#include "UI.h"

int main()
{
	testsDynamicArray();
	testsPlanetRepo();
	testsStack();

	Repo* repo = createRepo();
	OperationsStack* operationsStack = createStack();
	Controller* ctrl = createController(repo, operationsStack);

	
	addOfferCtrl(ctrl, "seaside", "mamaia", "15.05.18", 450);
	addOfferCtrl(ctrl, "mountain", "Padis", "07.05.18", 150);
	addOfferCtrl(ctrl, "seaside", "mamaia", "16.05.18", 800);
	addOfferCtrl(ctrl, "city break", "mamaia", "13.05.18", 300);
	addOfferCtrl(ctrl, "mountain", "Alps", "24.06.18", 900);
	addOfferCtrl(ctrl, "city break", "Dublin", "29.05.18", 500);
	addOfferCtrl(ctrl, "seaside", "Hawaii", "18.07.18", 1350);

	UI* ui = createUI(ctrl);

	startUI(ui);

	destroyUI(ui);

	return 0;
}
