package bijoux.xlserver.excel.xloper;

public final class XLNum extends XLoper {

	public final double num;

	public XLNum(double num) {
		super(xlTypeNum);
		this.num = num;
	}

	public String toString() {
		return Double.toString(num);
	}

	@Override
	public boolean equals ( Object other ) {
		XLNum otherNum = (XLNum)other;
		return otherNum.num == this.num;	
	}
}