/****************************************************************************
** Meta object code from reading C++ file 'graphView.h'
**
** Created: Sun 6. May 11:03:37 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../graphView.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'graphView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GraphView[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   11,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      47,   37,   10,   10, 0x0a,
      78,   72,   10,   10, 0x0a,
     112,  108,   10,   10, 0x0a,
     138,   10,   10,   10, 0x0a,
     166,  151,   10,   10, 0x0a,
     190,  185,   10,   10, 0x0a,
     206,   10,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_GraphView[] = {
    "GraphView\0\0image\0updateImage(QImage)\0"
    "nodesPere\0setNodes(QList<Element>)\0"
    "graph\0setGraph(QList<GraphElement>)\0"
    "dep\0setGraphEtape(QList<int>)\0"
    "clearGraph()\0widthV,heightV\0"
    "setTaille(int,int)\0zoom\0setZoom(double)\0"
    "paint()\0"
};

const QMetaObject GraphView::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_GraphView,
      qt_meta_data_GraphView, 0 }
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
        switch (_id) {
        case 0: updateImage((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 1: setNodes((*reinterpret_cast< QList<Element>(*)>(_a[1]))); break;
        case 2: setGraph((*reinterpret_cast< QList<GraphElement>(*)>(_a[1]))); break;
        case 3: setGraphEtape((*reinterpret_cast< QList<int>(*)>(_a[1]))); break;
        case 4: clearGraph(); break;
        case 5: setTaille((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: setZoom((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 7: paint(); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void GraphView::updateImage(QImage _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
