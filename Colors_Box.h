#ifndef BOI_BOX_H
#define BOI_BOX_H

#include <QObject>
#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QSqlQuery>
#include <QGridLayout>
#include <QMessageBox>
#include <QSqlError>

class Colors_Box : public QDialog
{
    Q_OBJECT
public:
    explicit Colors_Box(QDialog *parent = nullptr);
protected:
    void setupForm();

    QLineEdit *m_leBoja;
    QPushButton *m_dodadi;
    QPushButton *m_zatvori;

public slots:
    void slotDodadiClicked();
    void slotZatvoriClicked();

};

#endif // BOI_BOX_H
