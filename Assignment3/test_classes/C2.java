class Sum {
  public static void main(String[] a){
    System.out.println(new Test().Sum1(100));
  }
}

class Test {
  public int Sum1(int num) {
    int sum;
    sum = 0;
    while (0 < num) {
      if (50 < num && sum < 2000) {
    	  System.out.println(sum);
      } else {
    	  System.out.println(num);
      }
      sum = sum + num;
      num = num - 1;
    }
    return sum;
  }
}

