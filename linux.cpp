////////////////////////////////////////////////////////////////////////////
// linux.cpp -- Hiragana Calculator Calc-H for Linux
// This file is part of Calc-H.  See file "ReadMe.txt" and "License.txt".
////////////////////////////////////////////////////////////////////////////
// (Japanese, UTF-8)

#include "stdafx.h"
#include <thread>

////////////////////////////////////////////////////////////////////////////

class MainWindow : public Gtk::Window {
public:
    MainWindow();
    virtual ~MainWindow();

protected:
    virtual void on_button_enter();

    Gtk::VBox       m_vbox;
        Gtk::TextView   m_textview;
        Gtk::HBox       m_hbox;
            Gtk::Label      m_label;
            Gtk::Entry      m_entry;
            Gtk::Button     m_button_enter;

    Glib::RefPtr<Gtk::TextBuffer> m_text_buf;

    void add_output(const std::string& contents);
}; // class MainWindow

////////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow() {
    set_title("ひらがな電卓Calc-H ver.0.8.8");
    set_size_request(500, 280);

    Pango::FontDescription font_desc;
    #ifdef _WIN32
        font_desc.set_family("MS Gothic");
    #else
        font_desc.set_family("typewriter");
    #endif
    m_label.modify_font(font_desc);
    m_entry.modify_font(font_desc);
    m_textview.modify_font(font_desc);
    m_button_enter.modify_font(font_desc);

    set_border_width(4);
    m_hbox.set_border_width(8);
    m_vbox.set_border_width(8);

    m_text_buf = Gtk::TextBuffer::create();
    m_textview.set_buffer(m_text_buf);

    m_label.set_label("にゅうりょく:");
    m_entry.set_text("");

    m_button_enter.set_label("Enter");
    m_button_enter.signal_clicked().connect(
        sigc::mem_fun(*this, &MainWindow::on_button_enter)
    );

    m_hbox.pack_start(m_label, Gtk::PACK_SHRINK);
    m_hbox.pack_start(m_entry);
    m_hbox.pack_start(m_button_enter, Gtk::PACK_SHRINK);

    m_vbox.pack_start(m_textview);
    m_vbox.pack_start(m_hbox, Gtk::PACK_SHRINK);

    add(m_vbox);

    show_all_children();
    m_button_enter.set_can_default();

    set_focus(m_entry);

    std::string contents;
    contents += ch_logo;
    contents += '\n';
    add_output(contents);
} // MainWindow::MainWindow

MainWindow::~MainWindow() {
    ;
} // MainWindow::~MainWindow

void MainWindow::on_button_enter() {
    std::thread(
        [this](int dummy) {
            std::string query = m_entry.get_text();

            m_entry.set_sensitive(false);
            m_entry.set_text("計算中です...");

            CrTrimString(query);
            auto result = ChJustDoIt(query);
            ChFixResultForDisplay(result);

            std::string contents("にゅうりょく：");
            contents += query;
            contents += '\n';
            contents += result;
            contents += '\n';
            add_output(contents);

            if (result.find("しゅうりょうします") != std::string::npos) {
                m_entry.set_text("終了中です...");
                hide();
            }

            m_entry.set_text("");
            m_entry.set_sensitive(true);
            set_focus(m_entry);
        }, 0
    ).detach();
}

void MainWindow::add_output(const std::string& contents) {
    m_text_buf->set_text(m_text_buf->get_text() + contents);
    auto end = m_text_buf->get_iter_at_line(m_text_buf->get_line_count() + 100);
    m_textview.scroll_to(end);
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
