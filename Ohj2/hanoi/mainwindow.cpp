#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QGraphicsRectItem>
#include <cmath>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);
    scene_ = new QGraphicsScene(this);

    timer_ = new QTimer(this);
    connect(timer_, SIGNAL(timeout()), this, SLOT(seconds_passed()));

    ui_->graphicsView->setScene(scene_);

    // The graphicsView object is placed on the window
    // at the following coordinates:
    int left_margin = 10; // x coordinate
    int top_margin = 270; // y coordinate
    // The width of the graphicsView is BORDER_RIGHT added by 2,
    // since the borders take one pixel on each side
    // (1 on the left, and 1 on the right).
    // Similarly, the height of the graphicsView is BORDER_DOWN added by 2.
    ui_->graphicsView->setGeometry(left_margin, top_margin,
                                   BORDER_RIGHT + 2, BORDER_DOWN + 2);

    scene_->setSceneRect(0, 0, BORDER_RIGHT - 1, BORDER_DOWN - 1);

    ui_->minuteLCD->setPalette(Qt::blue);
    ui_->secondLCD->setPalette(Qt::blue);
    ui_->bestMinLCD->setPalette(Qt::green);
    ui_->bestSecLCD->setPalette(Qt::green);

    draw_the_poles();
}

MainWindow::~MainWindow()
{
    // deleting all of the disks created
    for (auto Disk : all_the_disks)
    {
        delete Disk;
    }
    delete ui_;
}

void MainWindow::on_startButton_clicked()
{
    /* When the start-button is clicked, we check the
     * entered disk amount for errors. If the disk
     * amount is playable, the user receives instructions,
     * the timer starts and we call the function that
     * draw the disks.
     */

    ui_->restartButton->setEnabled(true);

    QString disk_string = ui_->diskAmount->text();
    disk_amount_ = disk_string.toInt();

    // checking that the disk amount makes sense for the game
    if (disk_amount_ == 0)
    {
        ui_->infoLabel->setText("Error! The amound of disks cannot be zero");
    }
    else if (disk_amount_ > 10)
    {
        ui_->infoLabel->setText
                ("Error! The amount of disks cannot be over 10");
    }
    else
    {
        ui_->startButton->setDisabled(true);

        // reseting the time from a possible previous game
        ui_->minuteLCD->display(0);
        ui_->secondLCD->display(0);
        seconds_ = 0;
        minutes_ = 0;

        // calculating the minimum amount of movements needed to win
        minimum_amount_of_moves_ = pow(2, disk_amount_)-1;
        // displaying it in the GUI
        ui_->movesUsed->setText(QString::number(moves_));
        ui_->movesLeftNum->setText
                (QString::number(minimum_amount_of_moves_));

        ui_->infoLabel->setText("Use the number keys to move the disks:");
        ui_->keyLabel->setText("1: A-B, 2: B-C, 3: C-B, 4: B-A, 5: A-C "
                               "and 6: C-A");
        timer_->start(1000);
        draw_the_disks();
    }
}

void MainWindow::seconds_passed()
{
    // Keeps track of the time used so far
    // and displays it to the user
    ++seconds_;

    if (seconds_ >= 60)
    {
        ++minutes_;
        seconds_ = 0;
    }
    ui_->minuteLCD->display(minutes_);
    ui_->secondLCD->display(seconds_);

}

void MainWindow::draw_the_poles()
{
    // drawing the three poles
    for (int i = 0; i < 3; ++i)
    {
        // POLE_X is quarter of the width of the graphics scene
        // so the poles are at 1/4, 2/4 and 3/4 marks
        pole_ = scene_->addRect(POLE_X+(i*POLE_X), POLE_Y,
                                POLE_WIDTH, POLE_HEIGHT);

        std::vector <Disk* > pole_vector = {};
        Poles.push_back(pole_vector);
    }
}

void MainWindow::draw_the_disks()
{
    /* Draws N amount of disks and stores them
     * to a vector. The width of the disk is increased
     * every loop, while height stays constant, but
     * depends on the amount of disks.
     */

    qreal height = POLE_HEIGHT/disk_amount_;
    qreal width = DISK_WIDTH;

    qreal x = 0; // making the code easier to read
    qreal y = 0;

    for (unsigned int i = 0; i < disk_amount_; ++i)
    {
        Disk* new_disk = new Disk();

        // Making sure the disks aren't too wide to fit the scene.
        // The more disks there are, the narrower they are.
        width += i*(13-disk_amount_);

        x = 2*POLE_X-(width/2);
        y = POLE_Y+(height*i);

        new_disk->disk_ = scene_->addRect( x, y, width, height);
        new_disk->size_ = i;

        all_the_disks.push_back(new_disk);

        // adding the disks to the second pole vector
        Poles.at(1).push_back(new_disk);
    }
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    int i = 0; // index of the current pole
    int j = 1; // index of the target pole

    if (event->key() == Qt::Key_1)
        // moving the disk from pole A to B
    {
    }
    else if (event->key() == Qt::Key_2)
        // moving the disk from pole B to C
    {
        i = 1;
        j = 2;
    }
    else if (event->key() == Qt::Key_3)
        // moving the disk from pole C to B
    {
        i = 2;
        j = 1;
    }
    else if (event->key() == Qt::Key_4)
        // moving the disk from pole B to A
    {
        i = 1;
        j = 0;
    }
    else if (event->key() == Qt::Key_5)
        // moving the disk from pole A to C
    {
        i = 0;
        j = 2;
    }
    else if (event->key() == Qt::Key_6)
        // moving the disk from pole C to A
    {
        i = 2;
        j = 0;
    }

    std::vector <Disk*> pole = Poles.at(i);
    std::vector <Disk* > target_pole = Poles.at(j);

    if (pole.size() > 0)
    {
        // moving the smallest disk on the pole
        int index = find_smallest(pole);
        Disk* disk_to_move = pole.at(index);

        // if the next pole is empty, we can move the disk
        if (target_pole.size() == 0)
        {
            move_disk(disk_to_move, i, j);
        }
        // otherwise we must check the size of the disk
        // on top of the target pole
        else
        {
            int smallest_index = find_smallest(target_pole);
            Disk* disk_to_compare = target_pole.at(smallest_index);

            if (is_smaller(disk_to_move, disk_to_compare))
            {
                move_disk(disk_to_move, i, j);

            }else
            {
                ui_->infoLabel->setText("Error! An illegal move!");
                ui_->keyLabel->clear();
                on_restartButton_clicked();
            }
        }
    }
}

bool MainWindow::is_smaller(Disk*& disk_to_check, Disk*& disk_to_compare)
{
    return disk_to_check->size_< disk_to_compare->size_;
}

int MainWindow::find_smallest(std::vector <Disk*>& pole)
{
    // for keeping track of the smallest disk so far
    int smallest = pole.at(0)->size_;
    int smallest_index = 0;

    // if there's more than one element to inspect
    if (pole.size() > 1)
    {
        for (unsigned int i = 0; i < pole.size(); ++i)
        {
            if (pole.at(i)->size_ <= smallest)
            {
                smallest = pole.at(i)->size_;
                smallest_index = i;
            }
        }
    }return smallest_index;
}

void MainWindow::move_disk(Disk*& disk_to_move, const int& i, const int& j)
{
    qreal DELTA_X = POLE_X;
    // the distance to move (delta_x)
    // is positive if the index of the target pole
    // is greater than that of the current pole

    // adding the disk to the correct vector
    Poles.at(j).push_back(disk_to_move);

    // deleting the first element from the current pole
    // aka the top-most disk we just moved

    int to_be_deleted = find_smallest(Poles.at(i));
    Poles.at(i).erase(Poles.at(i).begin() + to_be_deleted);

    disk_to_move->disk_->moveBy((j-i)*DELTA_X, 0);

    moves_ += 1;
    int difference = minimum_amount_of_moves_ - moves_;
    ui_->movesUsed->setText(QString::number(moves_));
    ui_->movesLeftNum->setText(QString::number(difference));

    // if the first or last pole-vector contains all the
    // disks, the game is over
    if (Poles.at(j).size() == disk_amount_ && j != 1)
    {
        ui_->infoLabel->setText("You won! :)");
        ui_->keyLabel->clear();

        check_best_time();
        game_ended();

    }
}

void MainWindow::check_best_time()
{
    // checking for the best time so far
    if (minutes_ <= best_minutes_ && seconds_ < best_seconds_){

        best_minutes_ = minutes_;
        best_seconds_ = seconds_;
    }
    // if this is the first game
    else if (best_minutes_ == 0 && best_seconds_ == 0)
    {
        best_minutes_ = minutes_;
        best_seconds_ = seconds_;
    }
    ui_->bestMinLCD->display(best_minutes_);
    ui_->bestSecLCD->display(best_seconds_);
}

void MainWindow::game_ended()
{
    timer_->stop();

    ui_->diskAmount->clear();
    ui_->restartButton->setDisabled(true);
    ui_->startButton->setEnabled(true);

    minimum_amount_of_moves_ = 0;
    moves_ = 0;

    scene_->clear();

    // clear the poles of the old disks
    Poles = {};
    // redraw poles for the new game
    draw_the_poles();
}

void MainWindow::on_restartButton_clicked()
{
    game_ended();
}


