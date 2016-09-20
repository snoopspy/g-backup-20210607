#include "ggraphsignalslotform.h"

SignalSlotForm::SignalSlotForm(QWidget *parent) : QDialog(parent) {
	setControl();
}

SignalSlotForm::~SignalSlotForm() {
}

void SignalSlotForm::setControl()
{
  // ----- gilgil temp 2016.09.20 -----
  /*
	bool ok = true;
  if (ui->lwSignalList->selectedItems().count() == 0) ok = false;
  if (ui->lwSlotList->selectedItems().count() == 0)   ok = false;

	ui->btnOK->setEnabled(ok);
  */
  // ----------------------------------
}

void SignalSlotForm::on_btnOK_clicked()
{
	this->done(QDialog::Accepted);
}

void SignalSlotForm::on_btnCancel_clicked()
{
	this->done(QDialog::Rejected);
}

void SignalSlotForm::on_lwSignalList_currentRowChanged(int currentRow)
{
  (void)currentRow;
	setControl();
}

void SignalSlotForm::on_lwSlotList_currentRowChanged(int currentRow)
{
  (void)currentRow;
	setControl();
}

void SignalSlotForm::on_lwSignalList_clicked(const QModelIndex &index)
{
  (void)index;
	setControl();
}

void SignalSlotForm::on_lwSlotList_clicked(const QModelIndex &index)
{
  (void)index;
	setControl();
}
