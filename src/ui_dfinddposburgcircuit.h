/********************************************************************************
** Form generated from reading UI file 'dfinddposburgcircuit.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DFINDDPOSBURGCIRCUIT_H
#define UI_DFINDDPOSBURGCIRCUIT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpinBox>
#include <QtGui/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_DFindDPosBurgCircuit
{
public:
    QDialogButtonBox *buttonBox;
    QTextEdit *textEdit;
    QCheckBox *shuffleIdx;
    QCheckBox *interactive;
    QSpinBox *ncalcdpos;
    QLabel *label;

    void setupUi(QDialog *DFindDPosBurgCircuit)
    {
        if (DFindDPosBurgCircuit->objectName().isEmpty())
            DFindDPosBurgCircuit->setObjectName(QString::fromUtf8("DFindDPosBurgCircuit"));
        DFindDPosBurgCircuit->resize(561, 431);
        buttonBox = new QDialogButtonBox(DFindDPosBurgCircuit);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(140, 390, 241, 32));
        buttonBox->setLayoutDirection(Qt::LeftToRight);
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(true);
        textEdit = new QTextEdit(DFindDPosBurgCircuit);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setEnabled(true);
        textEdit->setGeometry(QRect(10, 20, 541, 281));
        textEdit->setAcceptDrops(false);
        textEdit->setAutoFillBackground(false);
        textEdit->setFrameShape(QFrame::NoFrame);
        textEdit->setFrameShadow(QFrame::Plain);
        textEdit->setMidLineWidth(0);
        textEdit->setReadOnly(true);
        textEdit->setAcceptRichText(false);
        textEdit->setTextInteractionFlags(Qt::NoTextInteraction);
        shuffleIdx = new QCheckBox(DFindDPosBurgCircuit);
        shuffleIdx->setObjectName(QString::fromUtf8("shuffleIdx"));
        shuffleIdx->setEnabled(true);
        shuffleIdx->setGeometry(QRect(10, 330, 531, 22));
        interactive = new QCheckBox(DFindDPosBurgCircuit);
        interactive->setObjectName(QString::fromUtf8("interactive"));
        interactive->setGeometry(QRect(10, 310, 531, 22));
        ncalcdpos = new QSpinBox(DFindDPosBurgCircuit);
        ncalcdpos->setObjectName(QString::fromUtf8("ncalcdpos"));
        ncalcdpos->setGeometry(QRect(370, 350, 53, 25));
        ncalcdpos->setMinimum(1);
        ncalcdpos->setMaximum(1000);
        ncalcdpos->setValue(1);
        label = new QLabel(DFindDPosBurgCircuit);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(120, 354, 241, 16));

        retranslateUi(DFindDPosBurgCircuit);
        QObject::connect(buttonBox, SIGNAL(accepted()), DFindDPosBurgCircuit, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DFindDPosBurgCircuit, SLOT(reject()));

        QMetaObject::connectSlotsByName(DFindDPosBurgCircuit);
    } // setupUi

    void retranslateUi(QDialog *DFindDPosBurgCircuit)
    {
        DFindDPosBurgCircuit->setWindowTitle(QApplication::translate("DFindDPosBurgCircuit", "Dislocation position using the Burgers circuit construction", 0, QApplication::UnicodeUTF8));
        textEdit->setHtml(QApplication::translate("DFindDPosBurgCircuit", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"justify\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Ubuntu'; font-size:11pt;\">This calculation will find a boundary of a convex region that contains in its interior the intersection of the dislocation line with the (x,y) plane shown (this can be any plane in the crystal). This boundary will be obtained by successive bisections of the initial convex region by the line segments that represent all pairs of atoms in the block. We are now going to define this region that contains the dislocation by picking a set of atoms that represent the corners of its boundary. </span><spa"
                        "n style=\" font-family:'Ubuntu'; font-size:11pt; color:#ff5500;\">These points have to be picked in a COUNTERCLOCKWISE direction and such that the polygon is CONVEX.</span></p>\n"
"<p align=\"justify\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Ubuntu'; font-size:11pt;\"><br /></p>\n"
"<p align=\"justify\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Ubuntu'; font-size:11pt;\">The relevant mouse events are:</span></p>\n"
"<p align=\"justify\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:600;\">click (left button)	</span><span style=\" font-family:'Ubuntu'; font-size:11pt;\">	picks an atom</span></p>\n"
"<p align=\"justify\" style=\" margin-top:0px; margin-bottom:0px;"
                        " margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:600;\">click (right button)</span><span style=\" font-family:'Ubuntu'; font-size:11pt;\">		removes the previously picked atom</span></p>\n"
"<p align=\"justify\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:600;\">double-click (left button)</span><span style=\" font-family:'Ubuntu'; font-size:11pt;\">	picks the last atom of the polygon</span></p>\n"
"<p align=\"justify\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Ubuntu'; font-size:11pt;\">			and runs the calculation</span></p>\n"
"<p align=\"justify\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:"
                        "0px; font-family:'Ubuntu'; font-size:11pt;\"><br /></p>\n"
"<p align=\"justify\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Ubuntu'; font-size:11pt;\">Click [Cancel] below if you don't want to continue.</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        shuffleIdx->setText(QApplication::translate("DFindDPosBurgCircuit", "Shuffle the order of atoms (Fisher-Yates algorithm)", 0, QApplication::UnicodeUTF8));
        interactive->setText(QApplication::translate("DFindDPosBurgCircuit", "Check this if you want to run interactive visualization of the calculation", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("DFindDPosBurgCircuit", "- number of shuffles (calculations):", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DFindDPosBurgCircuit: public Ui_DFindDPosBurgCircuit {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DFINDDPOSBURGCIRCUIT_H
