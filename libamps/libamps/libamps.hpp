#ifndef INC_LIBAMPS_LIBAMPS_HPP
#define INC_LIBAMPS_LIBAMPS_HPP

#include "media.hpp"

#include "arude/noncopyable.hpp"
#include "arude/sigslot.hpp"

#include <string>
#include <vector>


namespace libamps
{

///
/// Main class incorporating all of the functionality into one interface.
///
/// It is recommended to instatiate one instance of this class and keep it around
/// to the end.
///
class libamps final : arude::noncopyable
{
// Typedefs
public:

// Enums
public:
  ///
  /// Enumeration of search done flags.
  ///
  enum class searchdone_flag
  {
    finished,
    aborted
  };

// Structors
public:
  ///
  /// Default ctor.
  ///
  /// Tries to load or create the database and throws if not successfull.
  ///
  libamps() = default;

// Signals
public:
  ///
  /// Signal container.
  ///
  struct sig
  {
    arude::sigslot_signal<void()> on_itemfound;
    arude::sigslot_signal<void(searchdone_flag)> on_searchdone;
  };

// Accessors
public:
  ///
  /// Returns the signal container.
  /// \return Signal container
  ///
  const sig& signals() noexcept const;

  ///
  /// Returns the libamps version.
  /// \return String encoded version x.y.z
  ///
  const std::string& version() noexcept const;

  ///
  /// Returns the libamps data directory.
  /// For linux this is ~/.amps
  ///
  /// \return Data directory
  ///
  const std::experimental::path& data_directory() noexcept const;

  ///
  /// Returns a list of extension for the given media type \a mt.
  ///
  /// \param mt Media type
  /// \return List of extensions
  ///
  std::vector<std::string> extensions(media_type mt) const;

// Modifiers
public:
  ///
  /// Adds extension to media type.
  /// If the extension already exist for the given media type this is noop.
  ///
  /// \param mt Media type
  /// \param ext Extension
  ///
  void addextension(media_type mt, const std::string& ext);

  ///
  /// Remove extension from media type.
  /// If the extension doesn't exist for the given media type this is noop.
  ///
  /// \param mt Media type
  /// \param ext Extension
  ///
  void removeextension(media_type mt, const std::string& ext);

  ///
  /// Adds a path as include path to search media in.
  ///
  /// Such paths are always included recursevely with all child paths.
  /// If the path is an excluded path, it will be included.
  /// If the path is already included (maybe as sub path) this is noop.
  ///
  /// \param p Media path to include
  ///
  void addinclude_mediapath(const std::experimental::path& p);

  ///
  /// Adds a path as exclude path to not search media in.
  ///
  /// Such paths are always excluded recursevely with all child paths.
  /// If the path is an included path, it will be excluded.
  /// If the path is already excluded (maybe as sub path) this is noop.
  ///
  /// \param p Media path to exclude
  ///
  void addexclud_mediaepath(const std::experimental::path& p);

  ///
  /// Removes a path from being included or excluded.
  ///
  /// If the path isn't included or excluded this is noop.
  /// This must be a specific path, not a sub path, else this is noop.
  ///
  /// \param p Media path to remove
  ///
  void remove_mediapath(const std::experimental::path& p);

// Operations
public:
  ///
  /// Cleans the media database from invalid entries.
  /// Entries not existing anymore are removed.
  ///
  void cleandb();

  ///
  /// Adds non existing media to the db and updates existing if changed.
  ///
  void updatedb();

  ///
  /// Performs a \a cleandb & \a updatedb in sucession.
  ///
  void rebuilddb();

  ///
  /// Searches the database for media items matching the string \a str.
  ///
  /// For every media item found sig::on_itemfound is called.
  /// Every part of a media item is checked against the search string.
  ///
  /// \param str Search string
  /// \param maxhits Search stops after n media items where found
  ///
  ///
  void search(const std::string& str, size_t maxhits);

  ///
  /// Aborts a search in progress.
  /// Noop if no search is running.
  ///
  void abort_search();

// Variables
private:
  sig sig_; ///< Signal container instance.
};

} // namespace libamps

#endif // #ifndef INC_LIBAMPS_LIBAMPS_HPP
