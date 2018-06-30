#include <QMainWindow>
#include <QTextEdit>
#include <list>
#include <QList>

class TextEdit : public QTextEdit
{
    Q_OBJECT
public:
    TextEdit();

    explicit TextEdit(const QString &text);

protected:
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void keyPress(int key, int mod);
};


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();

public slots:
    // Функция проверки введеного символа
    void check_input_key(int key, int mod);

private slots:
    // Функция загрузки текста из файла
    void load_text();
    // Включить режим ТОША
    void set_mod_tosha(bool);

private:
    // Указатель на ui объект текстовой области
    TextEdit *text_block;

    // Список предложений
    QList <QString> text;
    // Флак загрузки текста
    bool isTextLoad = false;
    // Позиция курсора в текстовай области во время ввода
    int position = 0;
    // Указатель на текущее предложение
    QList<QString>::const_iterator line;
    // Выбранный режим, по умолчанию - 0
    int mod = 0;

    // Функция перехода на следующее предложение
    void load_next_line();
    // Сбросить текст
    void reset_text();
};