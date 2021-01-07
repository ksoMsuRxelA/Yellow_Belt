#include <iostream>
#include <map>
#include <tuple>


using namespace std;

// Перечислимый тип для статуса задачи
enum class TaskStatus {
    NEW,          // новая
    IN_PROGRESS,  // в разработке
    TESTING,      // на тестировании
    DONE          // завершена
};

// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = map<TaskStatus, int>;

class TeamTasks {
public:
//    TeamTasks(const string& name, int N, int INP, int T, int D) {
//        staff[name][TaskStatus::NEW] = N;
//        staff[name][TaskStatus::IN_PROGRESS] = INP;
//        staff[name][TaskStatus::TESTING] = T;
//        staff[name][TaskStatus::DONE] = D;
//    }
    // Получить статистику по статусам задач конкретного разработчика
    const TasksInfo& GetPersonTasksInfo(const string& person) const {
        return staff.at(person);
    }

    // Добавить новую задачу (в статусе NEW) для конкретного разработчитка
    void AddNewTask(const string& person) {
        if(staff.count(person) == 0) {
            staff[person][TaskStatus::NEW] = 1;
        } else {
            staff[person][TaskStatus::NEW]++;
        }
    }

    // Обновить статусы по данному количеству задач конкретного разработчика,
    // подробности см. ниже
    tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string& person, int task_count) {
        TasksInfo touched_tasks, untouched_tasks, current_tasks;
        if(staff.count(person) == 0) {
            return make_tuple(touched_tasks, untouched_tasks);
        } else {
            int non_completed_tasks = staff[person][TaskStatus::NEW]
                                    + staff[person][TaskStatus::IN_PROGRESS]
                                    + staff[person][TaskStatus::TESTING];
            if(task_count > non_completed_tasks) {
                task_count = non_completed_tasks;
            }
            int current_status = 0;
            while(task_count) {
                for(int i = 0; i < staff.at(person).at(static_cast<TaskStatus>(current_status))
                    && task_count > 0; ++i) {
                    touched_tasks[static_cast<TaskStatus>(current_status + 1)]++;
                    task_count--;
                }
                current_status++;
            }
            for(int i = 0; i < 3; ++i) { //mistake current_status.
                int tasks;
                if(touched_tasks.count(static_cast<TaskStatus>(i + 1)) != 0) {
                    tasks = staff.at(person).at(static_cast<TaskStatus>(i))
                            - touched_tasks.at(static_cast<TaskStatus>(i + 1));
                } else {
                    tasks = staff.at(person).at(static_cast<TaskStatus>(i));
                }
                if(tasks != 0) {
                    untouched_tasks[static_cast<TaskStatus>(i)] = tasks;
                } else {
                    continue;
                }
            }
            untouched_tasks[static_cast<TaskStatus>(3)] = staff.at(person)[static_cast<TaskStatus>(3)];
            for(int i = 0; i < 4; ++i) {
                int t, u;
                if(touched_tasks.count(static_cast<TaskStatus>(i)) == 0) {
                    t = 0;
                } else {
                    t = touched_tasks[static_cast<TaskStatus>(i)];
                }
                if(untouched_tasks.count(static_cast<TaskStatus>(i)) == 0) {
                    u = 0;
                } else {
                    u = untouched_tasks[static_cast<TaskStatus>(i)];
                }
                if(t + u != 0) {
                    current_tasks[static_cast<TaskStatus>(i)] = t + u;
                } else {
                    continue;
                }
            }
            staff.at(person) = current_tasks;
            untouched_tasks.erase(TaskStatus::DONE);
            return tie(touched_tasks, untouched_tasks);
        }
    }

private:
    map<string, TasksInfo> staff;
};

void PrintTasksInfo(TasksInfo tasks_info) {
    cout << tasks_info[TaskStatus::NEW] << " new tasks" <<
         ", " << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress" <<
         ", " << tasks_info[TaskStatus::TESTING] << " tasks are being tested" <<
         ", " << tasks_info[TaskStatus::DONE] << " tasks are done" << endl;
}

int main() {
    TeamTasks tasks;
    tasks.AddNewTask("Ilia");
    for (int i = 0; i < 3; ++i) {
        tasks.AddNewTask("Ivan");
    }
    cout << "Ilia's tasks: ";
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"));
    cout << "Ivan's tasks: ";
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"));

    TasksInfo updated_tasks, untouched_tasks;

    tie(updated_tasks, untouched_tasks) =
            tasks.PerformPersonTasks("Ivan", 2);
    cout << "Updated Ivan's tasks: ";
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: ";
    PrintTasksInfo(untouched_tasks);

    tie(updated_tasks, untouched_tasks) =
            tasks.PerformPersonTasks("Ivan", 2);
    cout << "Updated Ivan's tasks: ";
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: ";
    PrintTasksInfo(untouched_tasks);


    return 0;
}