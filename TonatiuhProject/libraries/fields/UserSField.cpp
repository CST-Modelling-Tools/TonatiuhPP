#include "UserSField.h"

SoType UserSField::classTypeId STATIC_SOTYPE_INIT;


SoType UserSField::getClassTypeId(void)
{
	return UserSField::classTypeId;
}

void UserSField::initClass(void)
{
    PRIVATE_FIELD_INIT_CLASS(UserSField, "UserSField", inherited, NULL);
}

void UserSField::atexit_cleanup(void)
{
    SoType::removeType(UserSField::classTypeId.getName());
    UserSField::classTypeId STATIC_SOTYPE_INIT;
}
