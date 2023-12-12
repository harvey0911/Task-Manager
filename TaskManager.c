#include <stdio.h>
#include <string.h>
int TaskNumber = 0;
#define size 10
typedef struct {

  char Task_Description[50];
  int Task_Number;
  double Time_to_finish;
} task_info;

void menu(void) {
  printf("\n\t\t------------- Menu --------------\n");
  printf("\n\t\t1.start the program\n");
  printf("\n\t\t2. update a task time \n");
  printf("\n\t\t3. add a new task\n");
  printf("\n\t\t4. act on the task in my queue  \n");
  printf("\n\t\t5. view tasks sorted\n");
  printf("\n\t\t6.Quit");
}

int search(task_info *min_heap, int task_to_update) {
  int i;
  // the function returns -1 if the task is not found. and it returns the index
  // in case it is found

  for (i = 0; i < size; i++) {

    if (min_heap[i].Task_Number == task_to_update)
      return i;
  }

  return -1;
}
task_info update(task_info *min_heap, int index, double time,
                 int add_or_deduct) {

  if (add_or_deduct == 1) {

    min_heap[index].Time_to_finish = min_heap[index].Time_to_finish + time;

  } else {
    min_heap[index].Time_to_finish = min_heap[index].Time_to_finish - time;
  }

  return min_heap[index];
}
void swap(task_info *X, task_info *Y) {
  task_info Temp;

  Temp = *X;
  *X = *Y;
  *Y = Temp;
}

void heapify(task_info *min_heap, int parent_index, int last_index) {
  int left_child_index, right_child_index, lowest_element_index;

  if (parent_index > (last_index - 1) / 2) { 

    return;
  } else {

    left_child_index = (2 * parent_index) + 1;
    right_child_index = (2 * parent_index) + 2;

    if (right_child_index > last_index)
      lowest_element_index = left_child_index;
    else {
      if (min_heap[left_child_index].Time_to_finish <
          min_heap[right_child_index].Time_to_finish)
        lowest_element_index = left_child_index;
      else
        lowest_element_index = right_child_index;
    }
    if (min_heap[parent_index].Time_to_finish >
        min_heap[lowest_element_index].Time_to_finish) {
      swap(&min_heap[parent_index], &min_heap[lowest_element_index]);
      heapify(min_heap, lowest_element_index, last_index);
    }
  }
}

int start_the_program(FILE *infp, task_info *min_heap) {

  int i = 0, last_index, number_of_tasks;
  char line[50];
  TaskNumber = 0;
  fseek(infp, 0, 0);
  while (!feof(infp) && i < size) {

    fgets(min_heap[i].Task_Description, 50, infp);
    min_heap[i].Task_Description[strlen(min_heap[i].Task_Description)-1]='\0';
    fscanf(infp, "%lf", &min_heap[i].Time_to_finish);
    getc(infp);
    fgets(line, 50, infp);
    min_heap[i].Task_Number = ++TaskNumber;
    i++;

  }

  number_of_tasks = i;
  last_index = number_of_tasks - 1;



  for (i = ((last_index - 1) / 2); i >= 0; i--) 
    heapify(min_heap, i, last_index);

  return number_of_tasks;
}

int find_level(int index) {

  int count = 0;
  while (index != 0) {

    index = (index - 1) / 2;
    count++;
  }

  return count;
}
void add_new_task(task_info* min_heap,task_info new_task,int number_of_tasks){
int index;
index=number_of_tasks-1;
    min_heap[number_of_tasks-1]=new_task;
    while(index>=1 && min_heap[index].Time_to_finish< min_heap[(index-1)/2].Time_to_finish){
        swap(&min_heap[index],&min_heap[(index-1)/2]);
        index=(index-1)/2;
}
}
task_info release(task_info* min_heap,int* number_of_tasks){

task_info to_return;

to_return=min_heap[0];
min_heap[0]=min_heap[*(number_of_tasks)-1];
(*number_of_tasks)--;

for (int j = (*number_of_tasks - 2) / 2; j >= 0; j--)
                     heapify(min_heap, j, *number_of_tasks - 1);

return to_return;
}

void heap_sort(task_info* min_heap,int number_of_tasks){
task_info temp[10];
int j=0;

    for(int i=0;i< number_of_tasks ;i++ )
            temp[i]=min_heap[i];


    while (number_of_tasks!=0)
    {
        min_heap[j]=release(temp,&number_of_tasks);
        j++;
    }



}
void Quit(FILE *infp,task_info* min_heap,int number_of_tasks){

for(int i=0;i<number_of_tasks;i++){
    fprintf(infp,"%s\n %2.f\n",min_heap[i].Task_Description,min_heap[i].Time_to_finish);
    fprintf(infp,"------------------\n");
}


}

int main(void) {
  int choice, flag = 0, number_of_tasks, index_or_not_found, task_to_update,
              add_or_deduct, level;
  double time, converted_time;
  char ch;
  task_info min_heap[size];
  task_info updated_task, new_task,released_task;
  FILE *infp;

  do {

    menu();
    scanf("%d", &choice);
    switch (choice) {

    case 1:
      flag = 1;
      infp = fopen("ToDoToday.txt", "r");
      if (infp == NULL) {

      } else {
        if (getc(infp) == EOF) {
          printf("the list of appointements is empty");
        } else {

          number_of_tasks = start_the_program(infp, min_heap);

          fclose(infp);



        }
      }

      break;
    case 2:

      if (flag != 1) {
        printf("you cannot update a task time before starting the program");
      } else {
        printf("enter the task number you want to update");
        scanf("%d", &task_to_update);

        index_or_not_found = search(min_heap, task_to_update);

        if (index_or_not_found == -1) {
          printf("this task does not exist in the heap");

        } else {

          printf("do you want to add or deduct time ?");
          scanf("%d", &add_or_deduct);

          if(add_or_deduct!=1){
            printf("enter the time you want to   deduct");
            do{
                scanf("%lf", &time);

                if(time>=min_heap[index_or_not_found].Time_to_finish)
                        printf("there is no tasks that requires %f time to finish ! please re enter the time to deduct",min_heap[index_or_not_found].Time_to_finish-time);


            }while(time>=min_heap[index_or_not_found].Time_to_finish); 

          }else{
                scanf("%lf", &time);
          }


          updated_task =update(min_heap, index_or_not_found, time, add_or_deduct);

          for (int j = (number_of_tasks - 2) / 2; j >= 0; j--)
            heapify(min_heap, j, number_of_tasks - 1);



          index_or_not_found = search(min_heap, updated_task.Task_Number);

          level = find_level(index_or_not_found);

          if (updated_task.Time_to_finish < 1)
            printf("Task %s with Number %d. has a time to finish:%.2f hours. "
                   "This task is found in level %d  of the heap!",updated_task.Task_Description, updated_task.Task_Number,
                   updated_task.Time_to_finish, level);
          else {
            converted_time = updated_task.Time_to_finish * 60;
            printf("Task %s with Number %d. has a time to finish:%.2f minutes. "
                   "This task is found in level %d of the heap!",updated_task.Task_Description, updated_task.Task_Number,
                   converted_time, level);
          }
        }
      }

      break;

    case 3:
      if (flag != 1) {
        printf("you cannot add a task time to a heap before starting the program");
      } else {

        if (number_of_tasks > size)
          printf("than no more tasks can be added!");
        else {




                printf("please enter the task decription");
                getchar();
                gets(new_task.Task_Description);


                new_task.Task_Number=++TaskNumber;
                number_of_tasks++;

                printf("please enter the time needed to accomplish this task ");
                scanf("%lf",&new_task.Time_to_finish);

                add_new_task(min_heap,new_task,number_of_tasks);

/*

   for (int  j = (number_of_tasks - 2) / 2; j >= 0; j--)
            heapify(min_heap, j, number_of_tasks - 1);*/


        }
      }
      break;
    case 4:
      if (flag != 1) {
        printf("you cannot release a task time before starting the program");
      } else {

        if(number_of_tasks==0){
            printf("there's no task left in the heap");
        }else{

            released_task=release(min_heap,&number_of_tasks);


                        if(released_task.Time_to_finish<1)
                    printf("task %s, was released from the heap. you will need %.f minutes to finish the task\n\n",released_task.Task_Description,released_task.Time_to_finish*60);
                        else
                    printf("task %s, was released from the heap. you will need %.f hours to finish the task\n\n",released_task.Task_Description,released_task.Time_to_finish);

        }



      }
      break;

    case 5:
      if (flag != 1) {
        printf("you cannot see  all the tasks  before starting the program");
      } else {

        heap_sort(min_heap,number_of_tasks);

        for(int j=0;j<number_of_tasks;j++){
              printf("%s\n %.2f\n",min_heap[j].Task_Description,min_heap[j].Time_to_finish);
              printf("----------------------\n");
            }


      }
      break;
    case 6:
     if (flag != 1) {
        printf("you cannot Quit  before starting the program");
      } else {

        if(number_of_tasks!=0){

                infp=fopen("ToDoNext.txt","w");
                Quit(infp,min_heap,number_of_tasks);
                    fclose(infp);
        }else{
            printf("you are done will all your tasks ! see you again ");
        }


      }
      break;
    }

  } while (choice != 6);
  return 0;
}
