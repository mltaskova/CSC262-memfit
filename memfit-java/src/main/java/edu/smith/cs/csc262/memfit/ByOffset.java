import java.util.Comparator;

/**
 * Created by mltaskova on 2/26/19.
 */
public class ByOffset implements Comparator<Block>{

    @Override
    public int compare(Block o1, Block o2) {
        return Integer.compare(o1.getOffset(), o2.getOffset());
    }
}
