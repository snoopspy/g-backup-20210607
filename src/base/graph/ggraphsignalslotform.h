// ----------------------------------------------------------------------------
//
// G Library
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#pragma once


#include <QDialog>
#include <QModelIndex>

// ----------------------------------------------------------------------------
// SignalSlotForm
// ----------------------------------------------------------------------------
class SignalSlotForm : public QDialog
{
	Q_OBJECT
	
public:
	explicit SignalSlotForm(QWidget *parent = 0);
  ~SignalSlotForm() override;

public:
	void setControl();
	
private slots:
	void on_btnOK_clicked();
	void on_btnCancel_clicked();
	void on_lwSignalList_currentRowChanged(int currentRow);
	void on_lwSlotList_currentRowChanged(int currentRow);
	void on_lwSignalList_clicked(const QModelIndex &index);
	void on_lwSlotList_clicked(const QModelIndex &index);
};
