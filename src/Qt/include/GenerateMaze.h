/**********************************************
 * @file GenerateMaze.h
 * @brief None
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.03.26
 * @license GNU General Public License (GPL)
 **********************************************/

#ifndef GENERATEMAZE_H
#define GENERATEMAZE_H

#include <QWidget>

namespace QT {
    QT_BEGIN_NAMESPACE

    namespace Ui {
        class GenerateMaze;
    }

    QT_END_NAMESPACE

    class GenerateMaze final : public QWidget {
        Q_OBJECT

    public:
        explicit GenerateMaze(QWidget *parent = nullptr);

        ~GenerateMaze() override;

    public slots:
        void saveMaze();

        void generateMaze();

    private:
        Ui::GenerateMaze *ui;
    };
} // QT

#endif //GENERATEMAZE_H
