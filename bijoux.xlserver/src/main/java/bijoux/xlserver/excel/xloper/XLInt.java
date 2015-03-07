package bijoux.xlserver.excel.xloper;

public class XLInt extends XLoper {

	public final int w;

	public XLInt(int w) {
		super(xlTypeInt);
		this.w = w;
	}

	public String toString() {
		return Integer.toString(w);
	}

	@Override
	public boolean equals ( Object other ) {
		XLInt otherInt = (XLInt)other;
		return otherInt.w == this.w;	
	}
}