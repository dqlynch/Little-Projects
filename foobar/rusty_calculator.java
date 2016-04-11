// Dylan Lynch
// 4/10/16
// foo.bar challenge 1

// Turns an inline expression into the lexicographically largest postfix
// expression representing the same thing
// Only works with single-digit numbers, with + and * operators

import java.util.Scanner;

public class rusty_calculator {

  public static void main(String args[]) {
    // Test answer()
    System.out.println("Enter an inline calculator statement with only ");
    System.out.print("single-digit numbers, '+', or '*': ");
    Scanner scan = new Scanner(System.in);
    String test = scan.next();
    System.out.println(answer(test));
  }

  public static String answer(String str) {
    if (findMultOp(str, 0, 0) < str.length()) {
      return processMults(str);   // recursively calls answer
    }
    else {
      return processAdds(str);
    }
  }

  public static String processMults(String str) {
    // Find mult. op group
    int multStart = findMultOp(str, 0, 0) - 1;    // start of multip group
    int multEnd = multStart + 2;                // starts at first op + 1
    while (isNextOp(str, multEnd, '*')) {
      multEnd += 2;
    }

    // Rearrange op. group
    return answer(turnPolish(str, multStart, multEnd));
  }

  public static String processAdds(String str) {
    // Just move all the +'s to the end
    String ops = str.replaceAll("[0-9*]", "");
    String theRest = str.replace("+", "");

    return theRest + ops;
  }

  // Return index of first non-polish multiplication operator (tail recursive)
  public static int findMultOp(String str, int prevNums, int index) {
    // Bounds check
    if (index == str.length()) {
      return str.length();
    }
    if (isNumber(str.charAt(index))) {
      // Found number
      return findMultOp(str, prevNums + 1, index + 1);
    }
    if (str.charAt(index) == '+') {
      // Restart count
      return findMultOp(str, 0, index + 1);
    }
    if (prevNums >= 2) {
      // Found processed operator
      return findMultOp(str, prevNums - 1, index + 1);
    }
    // Found unprocessed mult op
    return index;
  }

  // Rearrange the stuff between start and end to reverse polish for op
  public static String turnPolish(String str, int start, int end) {
    String substr = str.substring(start, end + 1); // include last num
    String theStart = str.substring(0, start);
    String theRest = str.substring(end + 1, str.length());

    String numbers = substr.replace("*", "");
    String ops = substr.replaceAll("[0-9]", "");

    return theStart + numbers + ops + theRest;
  }

  public static Boolean isNextOp(String str, int index, char op) {
    if (index + 1 >= str.length()) {
      return false;
    }
    if (str.charAt(index + 1) == op) {
      return true;
    }
    return false;
  }

  public static Boolean isNumber(char a) {
    return Character.isDigit(a);
  }
}
