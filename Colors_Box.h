#ifndef BOI_BOX_H
#define BOI_BOX_H

#include <QDialog>

class QLineEdit;

class Colors_Box : public QDialog
{
    Q_OBJECT
public:
    explicit Colors_Box(QWidget *parent = nullptr);
protected:
    void setupForm();

    QLineEdit *m_leColor;
    QPushButton *m_pbNew;
    QPushButton *m_pbClose;

public slots:
    void newClicked();
    void closeClicked();

};

#endif // BOI_BOX_H
