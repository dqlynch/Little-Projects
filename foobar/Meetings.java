// Dylan Lynch
// 4/12/2016

// Uses tree recursion to solve. Too slow for this problem, but fun
// implementation. Sort of a hybrid of knapsack problem and sieve.

// Evaluates the maximum number of meetings possible to take from an array
// of meetings

import java.util.Arrays;

public class Meetings {

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
  }

  // Tree recursively find max number of meetings
  public static int answer(int[][] meetings) {
    boolean[] canTake = new boolean[meetings.length]; // whether can take
    for (int i = 0; i < canTake.length; ++i) {        // can take unless marked
        canTake[i] = true;
    }
    return answer_helper(meetings, canTake, 0, 0);
  }

  public static int answer_helper(int[][] meetings, boolean[] canTake, int ind, int meetingCount) {

    // BASE CASE
    if (ind == meetings.length) {
      return meetingCount;
    }

    if (canTake[ind]) {
      if (alwaysTake(meetings, canTake, ind)) {
        return answer_helper(meetings, canTake, ind + 1, meetingCount + 1);
      }
      else {
        // Don't take this case
        int case_notTook = answer_helper(meetings, canTake, ind + 1, meetingCount);

        // Or take this case
        int case_took = answer_helper(meetings, markOffMeetings(meetings, canTake, ind), ind + 1, meetingCount + 1);

        return case_took > case_notTook ? case_took : case_notTook; // return max
      }
    }
    else {
      // Can't take this case
      return answer_helper(meetings, canTake, ind + 1, meetingCount);
    }
  }

  // Mark off meetings that the current meeting will make unavailable
  public static boolean[] markOffMeetings(int[][] meetings, boolean[] canTake, int ind) {
    int[] meeting = meetings[ind];
    boolean[] canTakeCopy = canTake.clone();
    for (int i = ind + 1; i < meetings.length; ++i) {
      if (meetingsOverlap(meeting, meetings[i]) && canTakeCopy[i]) {
        canTakeCopy[i] = false;
      }
    }
    return canTakeCopy;
  }

  // True if you should always take this case
  public static boolean alwaysTake(int[][] meetings, boolean[] canTake, int ind) {
    boolean[] canTakeCopy = markOffMeetings(meetings, canTake, ind);
    for (int i = ind; i < canTake.length; ++i) {
      if (canTake[i] != canTakeCopy[i]) {
        return false;
      }
    }
    return true;
  }

  // Returns true if meetings overlap, false otherwise
  public static boolean meetingsOverlap(int[] meeting1, int[] meeting2) {
    if (meeting2[0] >= meeting1[0] && meeting2[0] < meeting1[1]) {
      return true;
    }
    if (meeting1[0] >= meeting2[0] && meeting1[0] < meeting2[1]) {
      return true;
    }
    return false;
  }
}
