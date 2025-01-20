/**
 * @author ericfouh
 */
public class Similarities implements Comparable<Similarities> {
    /**
     * 
     */
    private String file1;
    private String file2;
    private int    count;


    /**
     * @param file1
     * @param file2
     */
    public Similarities(String file1, String file2) {
        this.file1 = file1;
        this.file2 = file2;
        this.setCount(0);
    }


    /**
     * @return the file1
     */
    public String getFile1() {
        return file1;
    }


    /**
     * @return the file2
     */
    public String getFile2() {
        return file2;
    }


    /**
     * @return the count
     */
    public int getCount() {
        return count;
    }


    /**
     * @param count the count to set
     */
    public void setCount(int count) {
        this.count = count;
    }


    @Override
    public int compareTo(Similarities o) {
        //TODO
        if (this.count != o.count) {
            return this.count - o.count;
        } else if (this.file1 != o.file1) {
            return this.file1.compareTo(o.file1);
        } else {
            return this.file2.compareTo(o.file2);
        }
    }

    @Override
    public String toString() {
        return "(" + this.file1 + "-" + this.file2 + " : " + this.count + ")";
    }
}
