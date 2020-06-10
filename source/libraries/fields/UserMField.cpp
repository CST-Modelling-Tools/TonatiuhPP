#include "UserMField.h"

SoType UserMField::classTypeId STATIC_SOTYPE_INIT;


void UserMField::initClass(void)
{
    PRIVATE_FIELD_INIT_CLASS(UserMField, "UserMField", inherited, NULL);
}

void UserMField::atexit_cleanup(void)
{
  SoType::removeType(UserMField::classTypeId.getName());
  UserMField::classTypeId STATIC_SOTYPE_INIT;
}
