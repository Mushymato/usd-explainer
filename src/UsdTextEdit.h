// custom QTextEdit for USD
#ifndef USDEDIT_H
#define USDEDIT_H

#include <QTextEdit>

class UsdTextEdit : public QTextEdit
{
    Q_OBJECT

public:
    UsdTextEdit(QWidget *parent = nullptr);

public slots:
    void setUsdFile(const QString &filePath);

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void printTextBlockData(QPoint pos);
    QTextCursor previousTextCursor;
};

#endif // USDEDIT_H
