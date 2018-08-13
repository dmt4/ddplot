/********************************************************************************
** Form generated from reading UI file 'ddposburgcircuitresults.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DDPOSBURGCIRCUITRESULTS_H
#define UI_DDPOSBURGCIRCUITRESULTS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_DDPosBurgCircuitResults
{
public:
    QDialogButtonBox *buttonBox;
    QCheckBox *showDPoly;
    QGroupBox *groupBox1;
    QTableWidget *coorNodes_rel;
    QGroupBox *groupBox1_1;
    QLabel *ydrel_tol;
    QLabel *label_3;
    QLabel *xdrel_tol;
    QLabel *label;
    QLabel *pm2;
    QLabel *pm1;
    QLabel *label_2;
    QLabel *xdrel;
    QLabel *ydrel;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *Bx;
    QLabel *By;
    QLabel *Bz;
    QLabel *label_10;
    QGroupBox *groupBox2;
    QLabel *label_16;
    QGroupBox *groupBox2_2;
    QLabel *ydinit_tol;
    QLabel *label_8;
    QLabel *xdinit_tol;
    QLabel *label_9;
    QLabel *pm2_2;
    QLabel *pm1_2;
    QLabel *label_11;
    QLabel *xdinit;
    QLabel *ydinit;
    QTableWidget *coorNodes_init;

    void setupUi(QDialog *DDPosBurgCircuitResults)
    {
        if (DDPosBurgCircuitResults->objectName().isEmpty())
            DDPosBurgCircuitResults->setObjectName(QString::fromUtf8("DDPosBurgCircuitResults"));
        DDPosBurgCircuitResults->resize(621, 651);
        buttonBox = new QDialogButtonBox(DDPosBurgCircuitResults);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(430, 610, 171, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        showDPoly = new QCheckBox(DDPosBurgCircuitResults);
        showDPoly->setObjectName(QString::fromUtf8("showDPoly"));
        showDPoly->setGeometry(QRect(20, 580, 471, 22));
        showDPoly->setChecked(true);
        showDPoly->setTristate(false);
        groupBox1 = new QGroupBox(DDPosBurgCircuitResults);
        groupBox1->setObjectName(QString::fromUtf8("groupBox1"));
        groupBox1->setGeometry(QRect(20, 10, 581, 251));
        coorNodes_rel = new QTableWidget(groupBox1);
        if (coorNodes_rel->columnCount() < 2)
            coorNodes_rel->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        coorNodes_rel->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        coorNodes_rel->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        coorNodes_rel->setObjectName(QString::fromUtf8("coorNodes_rel"));
        coorNodes_rel->setGeometry(QRect(20, 60, 214, 171));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        coorNodes_rel->setFont(font);
        coorNodes_rel->setAutoFillBackground(false);
        coorNodes_rel->setFrameShape(QFrame::StyledPanel);
        coorNodes_rel->setGridStyle(Qt::SolidLine);
        coorNodes_rel->setRowCount(0);
        coorNodes_rel->horizontalHeader()->setVisible(true);
        coorNodes_rel->horizontalHeader()->setDefaultSectionSize(98);
        coorNodes_rel->verticalHeader()->setVisible(false);
        coorNodes_rel->verticalHeader()->setDefaultSectionSize(20);
        groupBox1_1 = new QGroupBox(groupBox1);
        groupBox1_1->setObjectName(QString::fromUtf8("groupBox1_1"));
        groupBox1_1->setGeometry(QRect(250, 60, 311, 171));
        ydrel_tol = new QLabel(groupBox1_1);
        ydrel_tol->setObjectName(QString::fromUtf8("ydrel_tol"));
        ydrel_tol->setGeometry(QRect(210, 50, 67, 17));
        label_3 = new QLabel(groupBox1_1);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(60, 50, 67, 17));
        xdrel_tol = new QLabel(groupBox1_1);
        xdrel_tol->setObjectName(QString::fromUtf8("xdrel_tol"));
        xdrel_tol->setGeometry(QRect(210, 30, 67, 17));
        label = new QLabel(groupBox1_1);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 10, 191, 17));
        pm2 = new QLabel(groupBox1_1);
        pm2->setObjectName(QString::fromUtf8("pm2"));
        pm2->setGeometry(QRect(180, 50, 21, 17));
        pm1 = new QLabel(groupBox1_1);
        pm1->setObjectName(QString::fromUtf8("pm1"));
        pm1->setGeometry(QRect(180, 30, 21, 17));
        label_2 = new QLabel(groupBox1_1);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(60, 30, 67, 17));
        xdrel = new QLabel(groupBox1_1);
        xdrel->setObjectName(QString::fromUtf8("xdrel"));
        xdrel->setGeometry(QRect(100, 30, 81, 17));
        xdrel->setFont(font);
        ydrel = new QLabel(groupBox1_1);
        ydrel->setObjectName(QString::fromUtf8("ydrel"));
        ydrel->setGeometry(QRect(100, 50, 81, 17));
        ydrel->setFont(font);
        label_4 = new QLabel(groupBox1_1);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 80, 261, 17));
        label_5 = new QLabel(groupBox1_1);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(60, 100, 67, 17));
        label_6 = new QLabel(groupBox1_1);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(60, 120, 67, 17));
        label_7 = new QLabel(groupBox1_1);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(60, 140, 67, 17));
        Bx = new QLabel(groupBox1_1);
        Bx->setObjectName(QString::fromUtf8("Bx"));
        Bx->setGeometry(QRect(100, 100, 81, 17));
        Bx->setFont(font);
        By = new QLabel(groupBox1_1);
        By->setObjectName(QString::fromUtf8("By"));
        By->setGeometry(QRect(100, 120, 81, 17));
        By->setFont(font);
        Bz = new QLabel(groupBox1_1);
        Bz->setObjectName(QString::fromUtf8("Bz"));
        Bz->setGeometry(QRect(100, 140, 81, 17));
        Bz->setFont(font);
        label_10 = new QLabel(groupBox1);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(20, 30, 251, 21));
        groupBox2 = new QGroupBox(DDPosBurgCircuitResults);
        groupBox2->setObjectName(QString::fromUtf8("groupBox2"));
        groupBox2->setGeometry(QRect(20, 280, 581, 281));
        label_16 = new QLabel(groupBox2);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(20, 40, 251, 21));
        groupBox2_2 = new QGroupBox(groupBox2);
        groupBox2_2->setObjectName(QString::fromUtf8("groupBox2_2"));
        groupBox2_2->setGeometry(QRect(250, 70, 311, 81));
        ydinit_tol = new QLabel(groupBox2_2);
        ydinit_tol->setObjectName(QString::fromUtf8("ydinit_tol"));
        ydinit_tol->setGeometry(QRect(210, 50, 67, 17));
        label_8 = new QLabel(groupBox2_2);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(60, 50, 67, 17));
        xdinit_tol = new QLabel(groupBox2_2);
        xdinit_tol->setObjectName(QString::fromUtf8("xdinit_tol"));
        xdinit_tol->setGeometry(QRect(210, 30, 67, 17));
        label_9 = new QLabel(groupBox2_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(20, 10, 191, 17));
        pm2_2 = new QLabel(groupBox2_2);
        pm2_2->setObjectName(QString::fromUtf8("pm2_2"));
        pm2_2->setGeometry(QRect(180, 50, 21, 17));
        pm1_2 = new QLabel(groupBox2_2);
        pm1_2->setObjectName(QString::fromUtf8("pm1_2"));
        pm1_2->setGeometry(QRect(180, 30, 21, 17));
        label_11 = new QLabel(groupBox2_2);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(60, 30, 67, 17));
        xdinit = new QLabel(groupBox2_2);
        xdinit->setObjectName(QString::fromUtf8("xdinit"));
        xdinit->setGeometry(QRect(100, 30, 81, 17));
        xdinit->setFont(font);
        ydinit = new QLabel(groupBox2_2);
        ydinit->setObjectName(QString::fromUtf8("ydinit"));
        ydinit->setGeometry(QRect(100, 50, 81, 17));
        ydinit->setFont(font);
        coorNodes_init = new QTableWidget(groupBox2);
        if (coorNodes_init->columnCount() < 2)
            coorNodes_init->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        coorNodes_init->setHorizontalHeaderItem(0, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        coorNodes_init->setHorizontalHeaderItem(1, __qtablewidgetitem3);
        coorNodes_init->setObjectName(QString::fromUtf8("coorNodes_init"));
        coorNodes_init->setGeometry(QRect(20, 70, 214, 171));
        coorNodes_init->setFont(font);
        coorNodes_init->setAutoFillBackground(false);
        coorNodes_init->setFrameShape(QFrame::StyledPanel);
        coorNodes_init->setGridStyle(Qt::SolidLine);
        coorNodes_init->setRowCount(0);
        coorNodes_init->horizontalHeader()->setVisible(true);
        coorNodes_init->horizontalHeader()->setDefaultSectionSize(98);
        coorNodes_init->verticalHeader()->setVisible(false);
        coorNodes_init->verticalHeader()->setDefaultSectionSize(20);

        retranslateUi(DDPosBurgCircuitResults);
        QObject::connect(buttonBox, SIGNAL(accepted()), DDPosBurgCircuitResults, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DDPosBurgCircuitResults, SLOT(reject()));

        QMetaObject::connectSlotsByName(DDPosBurgCircuitResults);
    } // setupUi

    void retranslateUi(QDialog *DDPosBurgCircuitResults)
    {
        DDPosBurgCircuitResults->setWindowTitle(QApplication::translate("DDPosBurgCircuitResults", "Results of the calculation of the dislocation position", 0, QApplication::UnicodeUTF8));
        DDPosBurgCircuitResults->setWindowFilePath(QString());
        showDPoly->setText(QApplication::translate("DDPosBurgCircuitResults", "Show the boundary of the dislocation region in the main window", 0, QApplication::UnicodeUTF8));
        groupBox1->setTitle(QApplication::translate("DDPosBurgCircuitResults", "Computed in the deformed configuration", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = coorNodes_rel->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("DDPosBurgCircuitResults", "x", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = coorNodes_rel->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("DDPosBurgCircuitResults", "y", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        coorNodes_rel->setToolTip(QApplication::translate("DDPosBurgCircuitResults", "positions of atoms in units of the lattice parameter in the unrelaxed and relaxed configurations", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        ydrel_tol->setText(QApplication::translate("DDPosBurgCircuitResults", "0.0000", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("DDPosBurgCircuitResults", "yD =", 0, QApplication::UnicodeUTF8));
        xdrel_tol->setText(QApplication::translate("DDPosBurgCircuitResults", "0.0000", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("DDPosBurgCircuitResults", "Position of the dislocation:", 0, QApplication::UnicodeUTF8));
        pm2->setText(QApplication::translate("DDPosBurgCircuitResults", "+/-", 0, QApplication::UnicodeUTF8));
        pm1->setText(QApplication::translate("DDPosBurgCircuitResults", "+/-", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("DDPosBurgCircuitResults", "xD =", 0, QApplication::UnicodeUTF8));
        xdrel->setText(QApplication::translate("DDPosBurgCircuitResults", "+0.0000", 0, QApplication::UnicodeUTF8));
        ydrel->setText(QApplication::translate("DDPosBurgCircuitResults", "+0.0000", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("DDPosBurgCircuitResults", "Burgers vector inside of the polygon:", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("DDPosBurgCircuitResults", "Bx =", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("DDPosBurgCircuitResults", "By =", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("DDPosBurgCircuitResults", "Bz =", 0, QApplication::UnicodeUTF8));
        Bx->setText(QApplication::translate("DDPosBurgCircuitResults", "+0.0000", 0, QApplication::UnicodeUTF8));
        By->setText(QApplication::translate("DDPosBurgCircuitResults", "+0.0000", 0, QApplication::UnicodeUTF8));
        Bz->setText(QApplication::translate("DDPosBurgCircuitResults", "+0.0000", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("DDPosBurgCircuitResults", "Boundary of the dislocation region:", 0, QApplication::UnicodeUTF8));
        groupBox2->setTitle(QApplication::translate("DDPosBurgCircuitResults", "Mapping to the initial (undeformed) configuration", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("DDPosBurgCircuitResults", "Boundary of the dislocation region:", 0, QApplication::UnicodeUTF8));
        ydinit_tol->setText(QApplication::translate("DDPosBurgCircuitResults", "0.0000", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("DDPosBurgCircuitResults", "YD =", 0, QApplication::UnicodeUTF8));
        xdinit_tol->setText(QApplication::translate("DDPosBurgCircuitResults", "0.0000", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("DDPosBurgCircuitResults", "Position of the dislocation:", 0, QApplication::UnicodeUTF8));
        pm2_2->setText(QApplication::translate("DDPosBurgCircuitResults", "+/-", 0, QApplication::UnicodeUTF8));
        pm1_2->setText(QApplication::translate("DDPosBurgCircuitResults", "+/-", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("DDPosBurgCircuitResults", "XD =", 0, QApplication::UnicodeUTF8));
        xdinit->setText(QApplication::translate("DDPosBurgCircuitResults", "+0.0000", 0, QApplication::UnicodeUTF8));
        ydinit->setText(QApplication::translate("DDPosBurgCircuitResults", "+0.0000", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = coorNodes_init->horizontalHeaderItem(0);
        ___qtablewidgetitem2->setText(QApplication::translate("DDPosBurgCircuitResults", "X", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = coorNodes_init->horizontalHeaderItem(1);
        ___qtablewidgetitem3->setText(QApplication::translate("DDPosBurgCircuitResults", "Y", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        coorNodes_init->setToolTip(QApplication::translate("DDPosBurgCircuitResults", "positions of atoms in units of the lattice parameter in the unrelaxed and relaxed configurations", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
    } // retranslateUi

};

namespace Ui {
    class DDPosBurgCircuitResults: public Ui_DDPosBurgCircuitResults {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DDPOSBURGCIRCUITRESULTS_H
