/****************************************************************************
** Meta object code from reading C++ file 'protreewidget.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../protreewidget.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'protreewidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN13ProTreeWidgetE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN13ProTreeWidgetE = QtMocHelpers::stringData(
    "ProTreeWidget",
    "SigCancelProgress",
    "",
    "SlotItemPressed",
    "QTreeWidgetItem*",
    "item",
    "column",
    "SlotImport",
    "SlotSetActive",
    "SlotclosePro",
    "SlotUpdateProgress",
    "count",
    "SlotFinishProgress",
    "SlotCancelProgress",
    "SlotUpOpenProgress",
    "SlotFinishOpenProgress",
    "SlotCancelOPenProgress",
    "SlotOpenPro",
    "path"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN13ProTreeWidgetE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   86,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       3,    2,   87,    2, 0x08,    2 /* Private */,
       7,    0,   92,    2, 0x08,    5 /* Private */,
       8,    0,   93,    2, 0x08,    6 /* Private */,
       9,    0,   94,    2, 0x08,    7 /* Private */,
      10,    1,   95,    2, 0x08,    8 /* Private */,
      12,    0,   98,    2, 0x08,   10 /* Private */,
      13,    0,   99,    2, 0x08,   11 /* Private */,
      14,    1,  100,    2, 0x08,   12 /* Private */,
      15,    0,  103,    2, 0x08,   14 /* Private */,
      16,    0,  104,    2, 0x08,   15 /* Private */,
      17,    1,  105,    2, 0x0a,   16 /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 4, QMetaType::Int,    5,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   18,

       0        // eod
};

Q_CONSTINIT const QMetaObject ProTreeWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QTreeWidget::staticMetaObject>(),
    qt_meta_stringdata_ZN13ProTreeWidgetE.offsetsAndSizes,
    qt_meta_data_ZN13ProTreeWidgetE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN13ProTreeWidgetE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ProTreeWidget, std::true_type>,
        // method 'SigCancelProgress'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'SlotItemPressed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QTreeWidgetItem *, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'SlotImport'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'SlotSetActive'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'SlotclosePro'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'SlotUpdateProgress'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'SlotFinishProgress'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'SlotCancelProgress'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'SlotUpOpenProgress'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'SlotFinishOpenProgress'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'SlotCancelOPenProgress'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'SlotOpenPro'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
    >,
    nullptr
} };

void ProTreeWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ProTreeWidget *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->SigCancelProgress(); break;
        case 1: _t->SlotItemPressed((*reinterpret_cast< std::add_pointer_t<QTreeWidgetItem*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 2: _t->SlotImport(); break;
        case 3: _t->SlotSetActive(); break;
        case 4: _t->SlotclosePro(); break;
        case 5: _t->SlotUpdateProgress((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->SlotFinishProgress(); break;
        case 7: _t->SlotCancelProgress(); break;
        case 8: _t->SlotUpOpenProgress((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 9: _t->SlotFinishOpenProgress(); break;
        case 10: _t->SlotCancelOPenProgress(); break;
        case 11: _t->SlotOpenPro((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _q_method_type = void (ProTreeWidget::*)();
            if (_q_method_type _q_method = &ProTreeWidget::SigCancelProgress; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *ProTreeWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ProTreeWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN13ProTreeWidgetE.stringdata0))
        return static_cast<void*>(this);
    return QTreeWidget::qt_metacast(_clname);
}

int ProTreeWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void ProTreeWidget::SigCancelProgress()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
