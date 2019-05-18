void main() {
	// Video memory starts at 0xb8000
	char* video_memory = (char*) 0xb8000;
	// Display an 'X' in the top-left of the screen
	*video_memory = 'X';
}
