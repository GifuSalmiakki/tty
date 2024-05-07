/*
 * TIE-02201 Ohjelmointi 2: Perusteet Kesä 2020
 * Final GUI project: Hanoin Torni
 *
 * This program is a simple GUI representation of the game
 * "Tower of Hanoi", where the main objective is to move all the
 * disks from one pole to another, without breaking the order
 * of the disks i.e. not setting a bigger disk on top of a
 * smaller one.
 *
 * Programmer: Suvi Suominen, studentID: suomines,
 * e-mail: suvi.suominen@tuni.fi, studentNRO: 284432
 */

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH
#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include <QMouseEvent>
#include <vector>

namespace Ui
{
class MainWindow;
}

// This struct is to keep a record of
// what size the disk is.
struct Disk
{
    QGraphicsRectItem* disk_;
    int size_ = 0;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();
    void keyPressEvent(QKeyEvent* event) override;


private slots:
    void on_startButton_clicked();
    void seconds_passed();
    void on_restartButton_clicked();

private:
    Ui::MainWindow* ui_;
    QGraphicsScene* scene_;

    QTimer* timer_;
    // keeping track of the minutes and
    // seconds spent so far
    int seconds_ = 0;
    int minutes_ = 0;

    int best_seconds_ = 0;
    int best_minutes_ = 0;

    QGraphicsRectItem* pole_;
    QGraphicsRectItem* disk_;

    // storing the amount of disks the player
    // wants to use
    unsigned int disk_amount_ = 0;

    // storing the amount of moves the player has used
    // and comparing it to the minumum moves needed to win
    int moves_ = 0;
    int minimum_amount_of_moves_ = 0;

    // creating three vectors to represent the poles (in mainwindow.cpp)
    // and containing those in a vector as well
    std::vector < std::vector <Disk* >> Poles = {};
    // for deleting all of the disks at the end of the game
    std::vector <Disk*> all_the_disks = {};

    // coordinates for the first pole at the 1/4 mark
    const int POLE_X = 170;
    const int POLE_Y = 85;
    // and the height and width for all of them
    const int POLE_HEIGHT = 155;
    const int POLE_WIDTH = 3;
    // width of the disks
    const int DISK_WIDTH = 15;
   // const int DISK_HEIGHT = POLE_HEIGHT/disk_amount_;

    const int BORDER_UP = 0;
    const int BORDER_DOWN = 260;
    const int BORDER_LEFT = 0;
    const int BORDER_RIGHT = 680;

    // checks that the disk we want to move (disk_to_check) is
    // smaller than the top disk in the next pole (disk_to_compare
    bool is_smaller(Disk*& disk_to_check, Disk*& disk_to_compare);

    // moves the disk to the next pole
    // int i is the index of the current pole
    // int j is the target pole
    void move_disk(Disk*& disk_to_move, const int& i, const int& j);

    // drawing the disks at the beginning of a new game
    void draw_the_disks();

    // drawing the poles at the beginning of a new game
    void draw_the_poles();

    // the game ended either in victory or an illegal move
    void game_ended();

    // checking if the current time is better than the best
    // time so far, and changing it if so
    void check_best_time();

    // find the top-most disk of a pole aka the
    // one with the smallest size and returns its index
    int find_smallest(std::vector <Disk*>& pole);
};

#endif // MAINWINDOW_HH
