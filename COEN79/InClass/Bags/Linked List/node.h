class node {
    public:
		typedef _____ value_type; // specify data type
		node (const value_type& idata = value_type(), node* ilink = NULL) {
	        dataf = idata;
	        linkf = ilink;
        }
		const value_type& data() const { return dataf; }
		value_type& data() { return dataf; }
		const node* link() const { return linkf; }
		node*& link() { return linkf; }

	private:
		value_type dataf;
		node* linkf;
}