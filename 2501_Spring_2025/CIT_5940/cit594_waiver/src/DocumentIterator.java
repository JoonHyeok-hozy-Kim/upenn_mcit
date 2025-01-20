import java.io.IOException;
import java.io.Reader;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.NoSuchElementException;
import java.util.Queue;

public class DocumentIterator implements Iterator<String> {
    private final Reader r;
    private int    c = -1;
    private final int    n;
    private final Queue<String> nGramQueue;
    

    public DocumentIterator(Reader r, int n) {
        this.r = r;
        this.n = n;
        this.nGramQueue = new LinkedList<>();
        skipNonLetters();
    }


    private void skipNonLetters() {
        try {
            do {
                this.c = this.r.read();
            } while (!Character.isLetter(this.c) && this.c != -1);
        } catch (IOException e) {
            this.c = -1;
        }
    }


    @Override
    public boolean hasNext() {
        return (c != -1);
    }


    @Override
    public String next() {

        if (!hasNext()) {
            throw new NoSuchElementException();
        }
        String answer = "";        

        try {            
            while (this.nGramQueue.size() < this.n && hasNext()) {
                StringBuilder currWord = new StringBuilder();
                while (Character.isLetter(this.c)) {
                    // Covert to lower case if this.c is an upper case alphabet.
                    if (Character.isUpperCase(this.c)) {
                        this.c = Character.toLowerCase(this.c);
                    }
                    currWord.append((char) this.c);
                    this.c = this.r.read();
                }
                
                nGramQueue.offer(currWord.toString());
                skipNonLetters();
            }

        } catch (IOException e) {
            throw new NoSuchElementException();
        }

        if (this.nGramQueue.size() == this.n) {
            answer = String.join("", nGramQueue);
            nGramQueue.poll();
        }
        return answer;
    }
}
