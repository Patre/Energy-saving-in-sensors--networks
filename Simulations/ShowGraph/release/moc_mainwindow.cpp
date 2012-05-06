/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Sun 6. May 21:54:25 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      27,   11,   11,   11, 0x0a,
      43,   11,   11,   11, 0x0a,
      67,   11,   11,   11, 0x0a,
      83,   11,   11,   11, 0x0a,
      99,   11,   11,   11, 0x0a,
     112,   11,   11,   11, 0x0a,
     135,  129,   11,   11, 0x0a,
     157,  155,   11,   11, 0x0a,
     176,   11,   11,   11, 0x0a,
     193,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0chargerGraph()\0afficherNodes()\0"
    "afficherGrapheInitial()\0afficherEtape()\0"
    "afficherGraph()\0clearGraph()\0"
    "calculerDegree()\0image\0updateImage(QImage)\0"
    "x\0updateZoom(double)\0updateZoomPlus()\0"
    "updateZoomMoins()\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: chargerGraph(); break;
        case 1: afficherNodes(); break;
        case 2: afficherGrapheInitial(); break;
        case 3: afficherEtape(); break;
        case 4: afficherGraph(); break;
        case 5: clearGraph(); break;
        case 6: calculerDegree(); break;
        case 7: updateImage((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 8: updateZoom((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 9: updateZoomPlus(); break;
        case 10: updateZoomMoins(); break;
        default: ;
        }
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
