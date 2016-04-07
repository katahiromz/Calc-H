////////////////////////////////////////////////////////////////////////////
// linux.cpp -- Hiragana Calculator Calc-H for Linux
// This file is part of Calc-H.  See file "ReadMe.txt" and "License.txt".
////////////////////////////////////////////////////////////////////////////
// (Japanese, UTF-8)

#include "stdafx.h"

////////////////////////////////////////////////////////////////////////////

class MainWindow : public Gtk::Window {
public:
    MainWindow();
    virtual ~MainWindow();

protected:
    virtual void on_button_clicked();

    Gtk::VBox   m_vbox;
    Gtk::Label  m_label;
    Gtk::Button m_button;
}; // class MainWindow

////////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow() {
    set_title("TypicalGtkmmApp");
    set_size_request(300, 150);

    set_border_width(10);

    m_label.set_text("Hello, world!");

    m_button.set_label("Click me!");
    m_button.signal_clicked().connect(
        sigc::mem_fun(*this, &MainWindow::on_button_clicked)
    );

    m_vbox.pack_start(m_label);
    m_vbox.pack_start(m_button, Gtk::PACK_SHRINK);

    m_vbox.set_border_width(10);
    add(m_vbox);

    show_all_children();
} // MainWindow::MainWindow

MainWindow::~MainWindow() {
    ;
} // MainWindow::~MainWindow

void MainWindow::on_button_clicked() {
    printf("Hello, world\n");
    hide();
}

////////////////////////////////////////////////////////////////////////////

extern "C"
int main(int argc, char **argv) {
    Gtk::Main kit(argc, argv);
    MainWindow window;
    Gtk::Main::run(window);
    return 0;
}

////////////////////////////////////////////////////////////////////////////
