#include "settingsbar.h"
#include "ui_settingsbar.h"
#include "QDebug"
#include <QGraphicsView>
SettingsBar::SettingsBar(MainWindow *mainwindow, TextProvider *text_provider,
                         StatisticsProvider *statistics_provider, QStackedWidget *main_stackedWidget,
                         Form *form, User *user, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsBar)
{
    ui->setupUi(this);
    this->mainwindow = mainwindow;
    this->text_provider = text_provider;
    this->statistics_provider = statistics_provider;
    this->main_stackedWidget = main_stackedWidget;
    this->form = form;
    this->user = user;
    ui->register_panel->setCurrentIndex(0);

    ui->font_label->hide();
    ui->font_size_label->hide();
    ui->fontComboBox->hide();
    ui->font_size_comboBox->hide();

    for (int i = 5; i <= 40; i++)
        ui->font_size_comboBox->addItem(QString::number(i));
    ui->font_size_comboBox->setEditable(false);
    ui->font_size_comboBox->setCurrentIndex(20);

    ui->statistics_button->setFocusPolicy(Qt::NoFocus);

}

SettingsBar::~SettingsBar()
{
    delete ui;
}

void SettingsBar::on_settings_button_clicked()
{
    static bool is_shifted = 0;
    int shift = 80;

    QRect rect = ui->statistics_button->geometry();
    rect.setY(rect.y() + shift * (is_shifted ? -1 : 1));
    rect.setHeight(rect.height() + shift * (is_shifted ? -1 : 1));
    ui->statistics_button->setGeometry(rect);

    rect = ui->information_button->geometry();
    rect.setY(rect.y() + shift * (is_shifted ? -1 : 1));
    rect.setHeight(rect.height() + shift * (is_shifted ? -1 : 1));
    ui->information_button->setGeometry(rect);

    if (!is_shifted) {
        ui->font_label->show();
        ui->font_size_label->show();
        ui->fontComboBox->show();
        ui->font_size_comboBox->show();
    } else {
        ui->font_label->hide();
        ui->font_size_label->hide();
        ui->fontComboBox->hide();
        ui->font_size_comboBox->hide();
    }
    is_shifted ^= 1;
}

void SettingsBar::on_fontComboBox_currentFontChanged(const QFont &f)
{
    text_provider->change_font(new QFont(f));
}

void SettingsBar::on_font_size_comboBox_currentIndexChanged(const QString &arg1)
{
    text_provider->change_font_size(arg1.toInt());
}

void SettingsBar::on_statistics_button_clicked()
{
    if (ui->information_button->text() == "Go back")
        on_information_button_clicked();
    static bool is_statistics_opened = 1;
    main_stackedWidget->setCurrentIndex(is_statistics_opened);
    statistics_provider->display_statistics();
    ui->statistics_button->setText(is_statistics_opened ? "Go back" : "Statistics");
    is_statistics_opened ^= 1;
}

void SettingsBar::on_user_changed() {
    ui->user_name->setHtml("<p align=\"center\"><span style=\"color:#c0c0c0;\">" + user->name);
    ui->register_panel->setCurrentIndex(1 ^ ui->register_panel->currentIndex());
    if (ui->statistics_button->text() == "Go back") {
        statistics_provider->display_statistics();
    }
}

void SettingsBar::on_log_in_button_clicked()
{
    mainwindow->setEnabled(false);
    form->show(form->LogIn);
}

void SettingsBar::on_sign_up_button_clicked()
{
    mainwindow->setEnabled(false);
    form->show(form->SignUp);
}

void SettingsBar::on_log_out_button_clicked()
{
    *user = *(new User(user->db));
    ui->user_name->setText("Username");
    mainwindow->on_user_changed();
}

void SettingsBar::on_information_button_clicked()
{
    if (ui->statistics_button->text() == "Go back")
        on_statistics_button_clicked();
    static bool is_information_opened = 1;
    main_stackedWidget->setCurrentIndex(is_information_opened ? 2 : 0);
    statistics_provider->display_statistics();
    ui->information_button->setText(is_information_opened ? "Go back" : "Information");
    is_information_opened ^= 1;
}
