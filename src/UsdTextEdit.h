// custom QTextEdit for USD
#ifndef USDEDIT_H
#define USDEDIT_H

#include <QTextEdit>

class UsdTextEdit : public QTextEdit
{
    Q_OBJECT

public:
    //! constructor
    UsdTextEdit(QWidget *parent = nullptr);

public slots:
    //! open USD file
    void setUsdFile(const QString &filePath);

private:
    //! display tooltip for syntax matched in current text block
    void printTextBlockData(QTextCursor currentCursor, QPoint pos);
    //! callback, display tooltip when text cursor changes
    void handleTextCursorChange();
};

#endif // USDEDIT_H
