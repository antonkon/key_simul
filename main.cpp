#include "main.h"
//#include <iostream>

#include <QApplication>
#include <QPushButton>
#include <QMenuBar>
#include <QMenu>

#include <QFileDialog>
#include <QString>
#include <QChar>
#include <QFile>
#include <QDataStream>
#include <QIODevice>
#include <QMessageBox>
//#include <QTextStream>
#include <QKeyEvent>

TextEdit::TextEdit() : QTextEdit() {}
TextEdit::TextEdit(const QString &text) : QTextEdit(text) {}

void TextEdit::keyPressEvent(QKeyEvent *event) {
    emit keyPress(event->key(), event->modifiers());
}


MainWindow::MainWindow() {
    resize(640, 230);
    this->setWindowTitle(" ");

    QMenu *menu_file = new QMenu("___");
    menu_file->addAction("Открыть файл", this, SLOT(load_text()), Qt::CTRL + Qt:: Key_O);
    menu_file->addSeparator();
    menu_file->addAction("Выход", this, SLOT(close()), Qt::CTRL + Qt:: Key_Q);
    menuBar()->addMenu(menu_file);

    QMenu *menu_option = new QMenu("___");
    QAction *mod_tosha = new QAction("Режим \"ТОША\"", this);
    mod_tosha->setCheckable(true);
    mod_tosha->setChecked(true);
    connect(mod_tosha, SIGNAL(toggled(bool)), this, SLOT(set_mod_tosha(bool)));
    menu_option->addAction(mod_tosha);
    menuBar()->addMenu(menu_option);

    text_block = new TextEdit("");
    text_block->setStyleSheet("padding: 5px; background-color:white; font-size: 24px; color: black;");
    setCentralWidget(text_block);
    connect(text_block, SIGNAL(keyPress(int, int)),
            this, SLOT(check_input_key(int, int)));

    set_mod_tosha(1);
}

void MainWindow::load_text() {
    // Вызываем диалоговое окно для выбора файла
    QString file_name = QFileDialog::getOpenFileName(this, "Выберите файл с текстом", ".");

    // Проверяем что файл выбран
    if(file_name == nullptr)
        return;

    // Открываем выбранный выше файл
    QFile file(file_name);
    if(! file.open(QIODevice::ReadOnly)) {
        // Выводим сообщение об ошибки открытия файла
        QMessageBox::warning(this, "Ошибка открытия файла!", "Неудалось открыть файл %s"+
                                                             file.fileName()+
                                                             ":\n %s."+
                                                             file.errorString());
        return;
    }

    // Считываем файл, строка - элемент списка
    while(!file.atEnd())
    {
        text.append(file.readLine());
        text.last() = text.last().remove(QChar('\n'), Qt::CaseInsensitive);
        text.last() = text.last().remove(QChar('\r'), Qt::CaseInsensitive);
    }

    file.close();

    // Выставляем флак что текст загружен
    isTextLoad = true;

    // Указваем первое предложение
    line = text.constBegin();

    // Выводим текст на экран
    text_block->setText(*line);

}

void MainWindow::check_input_key(int key, int is_shift) {
    // Проверяем загружен ли текст
    if(!isTextLoad)
        return;

//    QTextStream out(stdout);
//    out << key << " ";
//    out << mod << " ";

    // Переводим код клавиши в символ
    QChar c = QChar(key);

    // Провиряем на функциональные клавиши
   if (key > 10000)
        return;

    // Проверяем на нажатие shift, если не нажат
    if (is_shift == 0) {
        // Преобразуем символ к нижнему регистру
        c = c.toLower();
    }
    // если - нажат, оставляем в верхнем

    // Проверяем введеный символ с символом в тексте
    switch(mod) {
        case 0:
            if (c == (*line)[position]) {
                // Если введеный символ правельный, то перемещаем курсор дальше по тексту
                position++;
                text_block->setText("<font color=\"#e2e2e2\">" + (*line).left(position) +
                                    "</font><font color=\"black\"><u>" + (*line)[position] + "</u>" +
                                    (*line).mid(position + 1)+"</font>");
            }
            break;

        case 1:
            if ((position >= (*line).length()) && (c == ' '))
                break;

            if (c == (*line)[position]) {
                // Если введеный символ правельный, то перемещаем курсор дальше по тексту
                position++;
            } else {
                position = 0;
                QApplication::beep();
            }
            text_block->setText("<font color=\"black\">" + (*line).left(position) +
                                "</font><font color=\"#e2e2e2\">" + (*line).mid(position) + "</font>");
            break;

        default:
            break;
    }

    // Проверяем на окончание предложения
    if((position == (*line).length()) && (c == ' '))
        // Переходим к следующему предложению
        load_next_line();
}

void MainWindow::load_next_line() {
    // Переводим указатель на след. предложение и
    // проверяем кончились ли предложения
    if (++line >= text.end()) {
        // Если - да
        // Уведомляем что текст закончился
        text_block->setText("<font size=\"18px\" color=\"red\">Текст закончился! Загрузите другой файл.</font>");
        // Выгружаем текст, приводим к состаянию - до загрузки текста
        reset_text();
    } else {
        // Если - нет
        // Переводим курсор в начало предложения
        position = 0;
        // Выводим следующее предложение
        text_block->setText(*line);
    }
}

void MainWindow::reset_text() {
    // Очищаем список предложений
    text.clear();
    // Снимаем флаг загрузки предложения
    isTextLoad = false;
    // Позицию возвращаем в начало строки
    position = 0;
    // Очишаем указатель на текущее предложение
    line = nullptr;
}

void MainWindow::set_mod_tosha(bool is_mod) {
    if(is_mod) {
        mod = 1;
    } else {
        mod = 0;
    }

}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow window;
    window.show();
    return app.exec();
}
