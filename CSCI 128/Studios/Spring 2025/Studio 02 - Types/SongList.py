search_song = input()
songs =  input().split(';')
artists = input().split(';')
albums = input().split(';')

index = songs.index(search_song)

print(f'{search_song}, by {artists[index]} ({albums[index]})')
