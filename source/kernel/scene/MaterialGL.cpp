#include "MaterialGL.h"

SO_NODE_SOURCE(MaterialGL)


void MaterialGL::initClass()
{
    SO_NODE_INIT_CLASS(MaterialGL, SoMaterial, "Material");
}

MaterialGL::MaterialGL()
{
    SO_NODE_CONSTRUCTOR(MaterialGL);
    isBuiltIn = true;
    SO_NODE_ADD_FIELD(reverseNormals, (FALSE) );
    SO_NODE_ADD_FIELD(ambientColor, (0.3, 0.4, 0.5) );
    SO_NODE_ADD_FIELD(diffuseColor, (0.3, 0.3, 0.3) );
    SO_NODE_ADD_FIELD(specularColor, (0.1, 0.1, 0.1) );
    SO_NODE_ADD_FIELD(shininess, (0.1) );

    // another option to create back material
    // https://coin3d.github.io/Coin/html/classSoIndexedFaceSet.html
}
