package bijoux.xlserver.excel.xloper;

public final class XLString extends XLoper {

	public static final XLString EMPTY = new XLString("");

	public final String          str;

	public XLString(String str) {
		super(xlTypeStr);
		this.str = trimToMaxLength(str);
	}

	private String trimToMaxLength(String str) {
		if (str == null)
			return "";
		if (str.length() > 255)
			return str.substring(0, 255);
		return str;
	}

	public String toString() {
		return str;
	}

	@Override
	public boolean equals ( Object other ) {
		XLString otherString = (XLString)other;
		return otherString.toString().equals(this.toString());
	}
}