/****************************************************************************
** Meta object code from reading C++ file 'graphView.h'
**
** Created: Sat Apr 28 11:25:19 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../ShowGraph/graphView.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'graphView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GraphView[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      21,   11,   10,   10, 0x0a,
      52,   46,   10,   10, 0x0a,
      86,   82,   10,   10, 0x0a,
     112,   10,   10,   10, 0x0a,
     131,  125,   10,   10, 0x0a,
     151,  147,   10,   10, 0x0a,
     170,  164,   10,   10, 0x0a,
     199,  187,   10,   10, 0x0a,
     232,  220,   10,   10, 0x0a,
     258,  253,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_GraphView[] = {
    "GraphView\0\0nodesPere\0setNodes(QList<Element>)\0"
    "graph\0setGraph(QList<GraphElement>)\0"
    "dep\0setGraphEtape(QList<int>)\0"
    "clearGraph()\0shape\0setShape(Shape)\0"
    "pen\0setPen(QPen)\0brush\0setBrush(QBrush)\0"
    "antialiased\0setAntialiased(bool)\0"
    "transformed\0setTransformed(bool)\0zoom\0"
    "setZoom(double)\0"
};

void GraphView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GraphView *_t = static_cast<GraphView *>(_o);
        switch (_id) {
        case 0: _t->setNodes((*reinterpret_cast< QList<Element>(*)>(_a[1]))); break;
        case 1: _t->setGraph((*reinterpret_cast< QList<GraphElement>(*)>(_a[1]))); break;
        case 2: _t->setGraphEtape((*reinterpret_cast< QList<int>(*)>(_a[1]))); break;
        case 3: _t->clearGraph(); break;
        case 4: _t->setShape((*reinterpret_cast< Shape(*)>(_a[1]))); break;
        case 5: _t->setPen((*reinterpret_cast< const QPen(*)>(_a[1]))); break;
        case 6: _t->setBrush((*reinterpret_cast< const QBrush(*)>(_a[1]))); break;
        case 7: _t->setAntialiased((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->setTransformed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->setZoom((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData GraphView::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject GraphView::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_GraphView,
      qt_meta_data_GraphView, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GraphView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GraphView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GraphView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GraphView))
        return static_cast<void*>(const_cast< GraphView*>(this));
    return QWidget::qt_metacast(_clname);
}

int GraphView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
