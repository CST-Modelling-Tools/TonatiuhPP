#pragma once

class QScriptValue;
class QScriptContext;
class QScriptEngine;

namespace tonatiuh_script
{
    int init(QScriptEngine* engine);

    QScriptValue tonatiuh_filename(QScriptContext* context, QScriptEngine* engine);
    QScriptValue tonatiuh_irradiance(QScriptContext* context, QScriptEngine* engine);
    QScriptValue tonatiuh_numrays(QScriptContext* context, QScriptEngine* engine);
    QScriptValue tonatiuh_numdivisions(QScriptContext* context, QScriptEngine* engine);
    QScriptValue tonatiuh_photon_map_export_mode(QScriptContext* context, QScriptEngine* engine);
    QScriptValue tonatiuh_random_generator(QScriptContext* context, QScriptEngine* engine);
    QScriptValue tonatiuh_sunposition(QScriptContext* context, QScriptEngine* engine);
    QScriptValue tonatiuh_calculatesun(QScriptContext* context, QScriptEngine* engine);
    QScriptValue tonatiuh_setsunpositiontoscene(QScriptContext* context, QScriptEngine* engine);
    QScriptValue tonatiuh_saveas(QScriptContext* context, QScriptEngine* engine);
    QScriptValue tonatiuh_trace(QScriptContext* context, QScriptEngine* engine);
}
