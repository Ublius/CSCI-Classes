public class CodingBatIndividual {
  // Author: Tristen Erpelding
  // Array-2 > tenRun
  public int[] tenRun(int[] nums) {
    for(int i = 0; i < nums.length; i++) {
      if (nums[i] % 10 == 0) {
        for(int j = i + 1; j < nums.length && nums[j] % 10 != 0; j++) {
          nums[j] = nums[i];
        }
      }
    }
  }

  // Author: ChatGPT
  // Array-2 > tenRun
  public int[] tenRun(int[] nums) {
    int[] result = new int[nums.length];
    int multipleOf10 = -1;  
    for (int i = 0; i < nums.length; i++) {
      if (nums[i] % 10 == 0) {
        multipleOf10 = nums[i];
      }
      result[i] = (multipleOf10 != -1) ? multipleOf10 : nums[i];
    }
    return result;
  }

  /* 
  Expected This Run
  tenRun([2, 10, 3, 4, 20, 5]) → [2, 10, 10, 10, 20, 20]	[2, 10, 10, 10, 20, 20]	OK	
  tenRun([10, 1, 20, 2]) → [10, 10, 20, 20]	[10, 10, 20, 20]	OK	
  tenRun([10, 1, 9, 20]) → [10, 10, 10, 20]	[10, 10, 10, 20]	OK	
  tenRun([1, 2, 50, 1]) → [1, 2, 50, 50]	[1, 2, 50, 50]	OK	
  tenRun([1, 20, 50, 1]) → [1, 20, 50, 50]	[1, 20, 50, 50]	OK	
  tenRun([10, 10]) → [10, 10]	[10, 10]	OK	
  tenRun([10, 2]) → [10, 10]	[10, 10]	OK	
  tenRun([0, 2]) → [0, 0]	[0, 0]	OK	
  tenRun([1, 2]) → [1, 2]	[1, 2]	OK	
  tenRun([1]) → [1]	[1]	OK	
  tenRun([]) → []	[]	OK	
  other tests OK	

  GPT Results
  tenRun([2, 10, 3, 4, 20, 5]) → [2, 10, 10, 10, 20, 20]	[2, 10, 10, 10, 20, 20]	OK	
  tenRun([10, 1, 20, 2]) → [10, 10, 20, 20]	[10, 10, 20, 20]	OK	
  tenRun([10, 1, 9, 20]) → [10, 10, 10, 20]	[10, 10, 10, 20]	OK	
  tenRun([1, 2, 50, 1]) → [1, 2, 50, 50]	[1, 2, 50, 50]	OK	
  tenRun([1, 20, 50, 1]) → [1, 20, 50, 50]	[1, 20, 50, 50]	OK	
  tenRun([10, 10]) → [10, 10]	[10, 10]	OK	
  tenRun([10, 2]) → [10, 10]	[10, 10]	OK	
  tenRun([0, 2]) → [0, 0]	[0, 0]	OK	
  tenRun([1, 2]) → [1, 2]	[1, 2]	OK	
  tenRun([1]) → [1]	[1]	OK	
  tenRun([]) → []	[]	OK	
  other tests OK

  My solution was very different from ChatGPT's I used a nested for loop which has a bigger complexity than ChatGPT's.
  ChatGPT's used a single for loop and a ternary operator.
  Both of our solutions were correct and passed all tests. 
  */

  // Author: Tristen Erpelding
  // AP-1 > scoresIncreasing
  public boolean scoresIncreasing(int[] scores) {
    for(int i = 0; i < scores.length; i++) {
      if (scores[i] < scores[i - 1]) {
        return false;
      }
    }
    return true;
  }

  // Author: ChatGPT
  // AP-1 > scoresIncreasing
  public boolean scoresIncreasing(int[] scores) {
    for(int i = 0; i < scores.length - 1; i++) {
      if (scores[i] > scores[i + 1]) {
        return false;
      }
    }
    return true;
  }

  /* 
  Expected This Run	
  scoresIncreasing([1, 3, 4]) → true	true	OK	
  scoresIncreasing([1, 3, 2]) → false	false	OK	
  scoresIncreasing([1, 1, 4]) → true	true	OK	
  scoresIncreasing([1, 1, 2, 4, 4, 7]) → true	true	OK	
  scoresIncreasing([1, 1, 2, 4, 3, 7]) → false	false	OK	
  scoresIncreasing([-5, 4, 11]) → true	true	OK	

  GPT Results
  scoresIncreasing([1, 3, 4]) → true	true	OK	
  scoresIncreasing([1, 3, 2]) → false	false	OK	
  scoresIncreasing([1, 1, 4]) → true	true	OK	
  scoresIncreasing([1, 1, 2, 4, 4, 7]) → true	true	OK	
  scoresIncreasing([1, 1, 2, 4, 3, 7]) → false	false	OK	
  scoresIncreasing([-5, 4, 11]) → true	true	OK

  My solution was very similar to ChatGPT's. Both solutions used a single for loop however the if statements were different.
  My way used a less than operator and ChatGPT's used a greater than operator.
  Both of our solutions were correct and passed all tests. 
  */
  
  // Author: Tristen Erpelding
  // String-2 > repeatEnd
  public String repeatEnd(String str, int n) {
    String repeatStr = "";
    for (int i = 0; i < n; i++) {
      repeatStr += str.substring(str.length() - n);
    }
    return repeatStr;
  }

  // Author: ChatGPT
  // String-2 > repeatEnd
  public String repeatEnd(String str, int n) {
    int length = str.length();
    StringBuilder result = new StringBuilder();
    for (int i = 0; i < n; i++) {
      result.append(str.substring(length - n));
    }
    return result.toString();
  }

  /*
  Expected This Run
  repeatEnd("Hello", 3) → "llollollo"	"llollollo"	OK	
  repeatEnd("Hello", 2) → "lolo"	"lolo"	OK	
  repeatEnd("Hello", 1) → "o"	"o"	OK	
  repeatEnd("Hello", 0) → ""	""	OK	
  repeatEnd("abc", 3) → "abcabcabc"	"abcabcabc"	OK	
  repeatEnd("1234", 2) → "3434"	"3434"	OK	
  repeatEnd("1234", 3) → "234234234"	"234234234"	OK	
  repeatEnd("", 0) → ""	""	OK	
  other tests OK

  GPT Results
  repeatEnd("Hello", 3) → "llollollo"	"llollollo"	OK	
  repeatEnd("Hello", 2) → "lolo"	"lolo"	OK	
  repeatEnd("Hello", 1) → "o"	"o"	OK	
  repeatEnd("Hello", 0) → ""	""	OK	
  repeatEnd("abc", 3) → "abcabcabc"	"abcabcabc"	OK	
  repeatEnd("1234", 2) → "3434"	"3434"	OK	
  repeatEnd("1234", 3) → "234234234"	"234234234"	OK	
  repeatEnd("", 0) → ""	""	OK	
  other tests OK

  My solution and ChatGPT's solution are very different. I used a for loop and a substring method to solve this problem.
  ChatGPT's solution used a StringBuilder and a for loop to solve this problem.
  Both of our solutions were correct and passed all tests. 
  */

  // Author: Tristen Erpelding
  // Array-3 > canBalance
  public boolean canBalance(int[] nums) {
    int sum = 0;
    for (int i = 0; i < nums.length; i++) {
      sum += nums[i];
      int sum2 = 0;
      for (int j = nums.length - 1; j > i; j--) {
        sum2 += nums[j];
      }
      if (sum == sum2) {
        return true;
      }
    }
    return false;
  }

  //Author: ChatGPT
  // Array-3 > canBalance
  public boolean canBalance(int[] nums) {
    int totalSum = 0;
    int currentSum = 0;
    for (int num : nums) {
      totalSum += num;
    }
    for (int num : nums) {
      currentSum += num;
        if (currentSum == totalSum / 2) {
          return true;
      }
    }
    return false;
  }

  /*
  Expected This Run
  canBalance([1, 1, 1, 2, 1]) → true	true	OK	
  canBalance([2, 1, 1, 2, 1]) → false	false	OK	
  canBalance([10, 10]) → true	true	OK	
  canBalance([10, 0, 1, -1, 10]) → true	true	OK	
  canBalance([1, 1, 1, 1, 4]) → true	true	OK	
  canBalance([2, 1, 1, 1, 4]) → false	false	OK	
  canBalance([2, 3, 4, 1, 2]) → false	false	OK	
  canBalance([1, 2, 3, 1, 0, 2, 3]) → true	true	OK	
  canBalance([1, 2, 3, 1, 0, 1, 3]) → false	false	OK	
  canBalance([1]) → false	false	OK	
  canBalance([1, 1, 1, 2, 1]) → true	true	OK	
  other tests   OK	

  GPT Results
  canBalance([1, 1, 1, 2, 1]) → true	true	OK	
  canBalance([2, 1, 1, 2, 1]) → false	true	X	
  canBalance([10, 10]) → true	true	OK	
  canBalance([10, 0, 1, -1, 10]) → true	true	OK	
  canBalance([1, 1, 1, 1, 4]) → true	true	OK	
  canBalance([2, 1, 1, 1, 4]) → false	true	X	
  canBalance([2, 3, 4, 1, 2]) → false	false	OK	
  canBalance([1, 2, 3, 1, 0, 2, 3]) → true	true	OK	
  canBalance([1, 2, 3, 1, 0, 1, 3]) → false	false	OK	
  canBalance([1]) → false	false	OK	
  canBalance([1, 1, 1, 2, 1]) → true	true	OK	
  other tests   X

  My solution and ChatGPT's solution are very different. I used a nested for loop to solve this problem.
  ChatGPT's solution used a single for loop and a ternary operator to solve this problem.
  My solution passed all tests but ChatGPT's solution did not.
  */
}