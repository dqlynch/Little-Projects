
import java.util.*;

public class Meetings2 {

  public static void main(String args[]) {
    // Test answer function
    int[][] meetings1 = {{0,10}, {1, 2}, {2, 3}};
    System.out.println("Expected: 2, Actual: " + answer(meetings1));

    int[][] meetings2 = {{0,1}, {1,2}, {2,3}, {0, 2}};
    System.out.println("Expected: 3, Actual: " + answer(meetings2));

    int[][] meetings3 = {{0, 1000000}, {1, 3}, {0, 1000000}, {1, 3}};
    System.out.println("Expected: 1, Actual: " + answer(meetings3));

    int[][] meetings4 = {{0, 1}, {5, 10}, {6, 7}, {7, 8}};
    System.out.println("Expected: 3, Actual: " + answer(meetings4));

    int[][] meetings5 = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5},
                         {10, 11}, {11, 12}, {12, 13}, {13, 14}, {14, 15},
                         {20, 21}, {21, 22}, {22, 23}, {23, 24}, {24, 25},
                         {30, 31}, {31, 32}, {32, 33}, {33, 34}, {34, 35},
                         {40, 41}, {41, 42}, {42, 43}, {43, 44}, {44, 45},
                         {50, 51}, {51, 52}, {52, 53}, {53, 54}, {54, 55},
                         {60, 61}, {61, 62}, {62, 63}, {63, 64}, {64, 65},
                         {70, 71}, {71, 72}, {72, 73}, {73, 74}, {74, 75}};
    System.out.println("Expected: 40, Actual: " + answer(meetings5));
  }

  public static int answer(int[][] meetings) {
    List<Meeting> meetingsList = arrayToList(meetings);
    sortList(meetingsList);

    // Determine which meetings you can take
    List<Meeting> takenList = new ArrayList<>();
    for (Meeting meeting : meetingsList) {
      checkIfCanTake(meeting, takenList);
    }
    // create taken list
    return takenList.size();
  }

  public static void checkIfCanTake(Meeting meeting,
                                    List<Meeting> takenList) {
    //
    for (Meeting takenMeeting : takenList) {
      if (meetingsOverlap(meeting, takenMeeting)) {
        return;
      }
    }
    takenList.add(meeting);
  }

  public static void sortList(List<Meeting> meetingsList) {
    Collections.sort(meetingsList, new Comparator<Meeting>() {
      @Override
      public int compare(Meeting lhs, Meeting rhs) {
        return lhs.endTime > rhs.endTime ? 1 : -1;  // cannot have eq endtimes
      }
    });
  }

  public static List<Meeting> arrayToList(int[][] meetings) {
    List<Meeting> meetingsList = new ArrayList<>();
    for (int i = 0; i < meetings.length; ++i) {
      Meeting meeting = new Meeting(i, meetings[i][0], meetings[i][1]);
      meetingsList.add(meeting);
    }
    return meetingsList;
  }

  public static boolean meetingsOverlap(Meeting m1, Meeting m2) {
    if (m2.startTime < m1.endTime &&
        m2.endTime > m1.startTime) {
      return true;
    }
    return false;
  }

  // Basically a struct with a constructor
  public static class Meeting {
    public int index;
    public int startTime;
    public int endTime;

    // Constructor
    public Meeting(int index_in, int startTime_in, int endTime_in) {
      index = index_in;
      startTime = startTime_in;
      endTime = endTime_in;
    }
  }
}
